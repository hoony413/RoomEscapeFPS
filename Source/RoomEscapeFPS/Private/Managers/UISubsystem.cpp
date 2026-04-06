// Fill out your copyright notice in the Description page of Project Settings.

#include "Managers/UISubsystem.h"
#include "Managers/UISettings.h"
#include "Helper/DataTableStructure.h"
#include "Engine/DataTable.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/UserWidgetPool.h"
#include "UI/LoadingScreen.h"
#include "UI/BaseHUDWidget.h"
#include "CommonActivatableWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "GameFramework/RoomEscapeFPSHUD.h"
#include "Widgets/CommonActivatableWidgetContainer.h"

void UUISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const UUISettings* settings = UUISettings::Get();
	if (not settings)
	{
		return;
	}
	UDataTable* dataTable = settings->WidgetDataTable.LoadSynchronous();
	if (not dataTable)
	{
		return;
	}
	TArray<FActivatableWidgetInfo*> rows;
	dataTable->GetAllRows<FActivatableWidgetInfo>(TEXT("UISubsystem::Initialize"), rows);
	for (FActivatableWidgetInfo const* row : rows)
	{
		if (row && EActivatableWidgetType::NONE != row->WidgetType)
		{
			_widgetClassCache.Add(row->WidgetType, { row->WidgetClass, row->LayerType });
		}
	}
}

void UUISubsystem::Deinitialize()
{
	HideLoadingScreen();
	_activeWidgets.Empty();
	_popupWidgetPool.ReleaseAll();
	_popupWidgetPool.ResetPool();
	Super::Deinitialize();
}

void UUISubsystem::AddReferencedObjects(UObject* InThis, FReferenceCollector& Collector)
{
	UUISubsystem* This = CastChecked<UUISubsystem>(InThis);
	This->_popupWidgetPool.AddReferencedObjects(Collector);
	Super::AddReferencedObjects(InThis, Collector);
}

void UUISubsystem::ShowLoadingScreen()
{
	if (not LoadingWidget)
	{
		const UUISettings* settings = UUISettings::Get();
		if (not settings)
		{
			return;
		}
		UClass* widgetClass = settings->LoadingWidgetClass.LoadSynchronous();
		if (not widgetClass)
		{
			return;
		}
		// GameInstance를 outer로 생성하여 월드 전환 시에도 위젯이 유지된다.
		LoadingWidget = CreateWidget<ULoadingScreen>(GetGameInstance(), widgetClass);
	}
	if (LoadingWidget && not LoadingWidget->IsInViewport())
	{
		LoadingWidget->AddToViewport(MAX_int32);
	}
}

void UUISubsystem::HideLoadingScreen()
{
	if (LoadingWidget && LoadingWidget->IsInViewport())
	{
		LoadingWidget->RemoveFromParent();
	}
}

UCommonActivatableWidgetStack* UUISubsystem::GetWidgetStack() const
{
	UWorld* world = GetWorld();
	if (not world)
	{
		return nullptr;
	}
	APlayerController* pc = world->GetFirstPlayerController();
	if (not pc)
	{
		return nullptr;
	}
	ARoomEscapeFPSHUD* hud = Cast<ARoomEscapeFPSHUD>(pc->GetHUD());
	if (not hud)
	{
		return nullptr;
	}
	UBaseHUDWidget* hudWidget = hud->GetHUDWidget();
	if (not hudWidget)
	{
		return nullptr;
	}
	return hudWidget->GetWidgetStack();
}

UCanvasPanel* UUISubsystem::GetPopupPanel() const
{
	UWorld* world = GetWorld();
	if (not world)
	{
		return nullptr;
	}
	APlayerController* pc = world->GetFirstPlayerController();
	if (not pc)
	{
		return nullptr;
	}
	ARoomEscapeFPSHUD* hud = Cast<ARoomEscapeFPSHUD>(pc->GetHUD());
	if (not hud)
	{
		return nullptr;
	}
	UBaseHUDWidget* hudWidget = hud->GetHUDWidget();
	if (not hudWidget)
	{
		return nullptr;
	}
	return hudWidget->GetPopupPanel();
}

void UUISubsystem::AddToPopupPanel(UUserWidget* widget)
{
	if (not widget)
	{
		return;
	}
	UCanvasPanel* popupPanel = GetPopupPanel();
	if (not popupPanel)
	{
		widget->AddToPlayerScreen();
		return;
	}
	if (UCanvasPanelSlot* slot = Cast<UCanvasPanelSlot>(popupPanel->AddChild(widget)))
	{
		slot->SetAnchors(FAnchors(0.f, 0.f, 1.f, 1.f));
		slot->SetOffsets(FMargin(0.f));
	}
}

void UUISubsystem::CloseWidget(UUserWidget* widget)
{
	if (not widget)
	{
		return;
	}

	if (auto* const activatable = Cast<UCommonActivatableWidget>(widget))
	{
		if (activatable->IsActivated())
		{
			activatable->DeactivateWidget();
		}
	}

	// _activeWidgets에서 위젯 타입을 찾고 제거
	EActivatableWidgetType widgetType = EActivatableWidgetType::NONE;
	for (auto it = _activeWidgets.CreateIterator(); it; ++it)
	{
		if (it->Value.Get() == widget)
		{
			widgetType = it->Key;
			it.RemoveCurrent();
			break;
		}
	}

	// FULL_SCREEN은 스택이 제거 처리하므로 추가 작업 불필요
	if (EActivatableWidgetLayerType::FULL_SCREEN == GetLayerType(widgetType))
	{
		return;
	}

	// POPUP: 풀 반납 후 부모에서 제거
	_popupWidgetPool.Release(widget);
	widget->RemoveFromParent();
}

UClass* UUISubsystem::GetWidgetClass(EActivatableWidgetType widgetType)
{
	FWidgetCacheInfo const* found = _widgetClassCache.Find(widgetType);
	return found ? found->WidgetClass.LoadSynchronous() : nullptr;
}

EActivatableWidgetLayerType UUISubsystem::GetLayerType(EActivatableWidgetType widgetType) const
{
	FWidgetCacheInfo const* found = _widgetClassCache.Find(widgetType);
	if (not found)
	{
		return EActivatableWidgetLayerType::NONE;
	}
	return found->LayerType;
}

UUserWidget* UUISubsystem::GetWidget_Internal(EActivatableWidgetType widgetType)
{
	UClass* widgetClass = GetWidgetClass(widgetType);
	if (not widgetClass)
	{
		return nullptr;
	}

	UWorld* world = GetWorld();
	if (not world)
	{
		return nullptr;
	}

	// 월드 전환 이후에도 올바른 월드에서 위젯이 생성되도록 매번 갱신한다.
	_popupWidgetPool.SetWorld(world);
	return _popupWidgetPool.GetOrCreateInstance<UUserWidget>(widgetClass);
}

UUserWidget* UUISubsystem::OpenWidget_Internal(EActivatableWidgetType widgetType)
{
	UClass* widgetClass = GetWidgetClass(widgetType);
	if (not widgetClass)
	{
		return nullptr;
	}

	EActivatableWidgetLayerType const layerType = GetLayerType(widgetType);

	if (EActivatableWidgetLayerType::FULL_SCREEN == layerType)
	{
		UCommonActivatableWidgetStack* stack = GetWidgetStack();
		if (not stack)
		{
			return nullptr;
		}
		stack->AddWidget(TSubclassOf<UCommonActivatableWidget>(widgetClass));
		UUserWidget* widget = stack->GetActiveWidget();
		if (widget)
		{
			_activeWidgets.Add(widgetType, widget);
		}
		return widget;
	}

	// POPUP 경로
	UUserWidget* widget = GetWidget_Internal(widgetType);
	if (not widget)
	{
		return nullptr;
	}

	if (not widget->IsInViewport())
	{
		AddToPopupPanel(widget);
	}
	else
	{
		widget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	
	if (auto* const activatable = Cast<UCommonActivatableWidget>(widget))
	{
		if (not activatable->IsActivated())
		{
			activatable->ActivateWidget();
		}
	}
	_activeWidgets.Add(widgetType, widget);
	return widget;
}

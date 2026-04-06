// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Blueprint/UserWidgetPool.h"
#include "Helper/DataTableStructure.h"
#include "UI/LoadingScreen.h"
#include "UISubsystem.generated.h"

class UCommonActivatableWidgetStack;
class UCanvasPanel;

/**
 * UI를 편하게 생성/호출하기 위한 GameInstanceSubsystem.
 * 위젯 클래스는 UISettings의 DataTable에서 관리한다.
 *
 * FULL_SCREEN 위젯은 WidgetStack이 생성/관리하고,
 * POPUP 위젯은 위젯 풀 + PopupPanel로 관리한다.
 * 공개 API는 OpenWidget / CloseWidget 단일 파이프라인만 제공.
 */
UCLASS()
class ROOMESCAPEFPS_API UUISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	void Initialize(FSubsystemCollectionBase& Collection) override;
	void Deinitialize() override;
	static void AddReferencedObjects(UObject* InThis, FReferenceCollector& Collector);

	void ShowLoadingScreen();
	void HideLoadingScreen();

	// GetWidget + 레이어 타입에 따라 WidgetStack 또는 PopupPanel에 배치한다.
	template<typename T>
	T* OpenWidget(EActivatableWidgetType widgetType)
	{
		return Cast<T>(OpenWidget_Internal(widgetType));
	}

	// 사용이 끝난 위젯을 닫는다. FULL_SCREEN은 스택이, POPUP은 풀이 처리한다.
	void CloseWidget(UUserWidget* widget);

	// 현재 열려 있는 위젯을 반환한다. 열려 있지 않으면 nullptr.
	template<typename T>
	T* GetActiveWidget(EActivatableWidgetType widgetType) const
	{
		TWeakObjectPtr<UUserWidget> const* found = _activeWidgets.Find(widgetType);
		if (found && found->IsValid())
		{
			return Cast<T>(found->Get());
		}
		return nullptr;
	}

private:
	struct FWidgetCacheInfo
	{
		TSoftClassPtr<UUserWidget> WidgetClass;
		EActivatableWidgetLayerType LayerType{};
	};

	UClass* GetWidgetClass(EActivatableWidgetType widgetType);
	EActivatableWidgetLayerType GetLayerType(EActivatableWidgetType widgetType) const;

	UCommonActivatableWidgetStack* GetWidgetStack() const;
	UCanvasPanel* GetPopupPanel() const;

	// 위젯을 PopupPanel의 자식으로 추가한다.
	void AddToPopupPanel(UUserWidget* widget);

	UUserWidget* GetWidget_Internal(EActivatableWidgetType widgetType);
	UUserWidget* OpenWidget_Internal(EActivatableWidgetType widgetType);

	FUserWidgetPool _popupWidgetPool;

	UPROPERTY(Transient)
	TObjectPtr<ULoadingScreen> LoadingWidget;

	TMap<EActivatableWidgetType, FWidgetCacheInfo> _widgetClassCache;

	// 현재 열려 있는 위젯 추적
	UPROPERTY(Transient)
	TMap<EActivatableWidgetType, TWeakObjectPtr<UUserWidget>> _activeWidgets;
};

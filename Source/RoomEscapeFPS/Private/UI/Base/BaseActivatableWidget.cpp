// Fill out your copyright notice in the Description page of Project Settings.
#include "UI/Base/BaseActivatableWidget.h"
#include "Runtime/UMG/Public/Blueprint/WidgetBlueprintLibrary.h"

void UBaseActivatableWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	SetWidgetAnimation();
}

void UBaseActivatableWidget::NativeOnActivated()
{
	Super::NativeOnActivated();

	ApplyDesiredInputConfig();

	SetViewModel();
	PostSetViewModel();
}

void UBaseActivatableWidget::NativeOnDeactivated()
{
	RestoreInputConfig();

	ClearViewModel();
	Super::NativeOnDeactivated();
}

void UBaseActivatableWidget::ApplyDesiredInputConfig()
{
	TOptional<FUIInputConfig> const config = GetDesiredInputConfig();
	if (not config.IsSet())
	{
		return;
	}

	APlayerController* pc = GetOwningPlayer();
	if (not pc)
	{
		return;
	}

	switch (config->GetInputMode())
	{
	case ECommonInputMode::Menu:
		UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(pc, this);
		pc->SetShowMouseCursor(true);
		break;
	case ECommonInputMode::All:
		UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(pc, this);
		pc->SetShowMouseCursor(true);
		break;
	default:
		break;
	}
}

void UBaseActivatableWidget::RestoreInputConfig()
{
	TOptional<FUIInputConfig> const config = GetDesiredInputConfig();
	if (not config.IsSet())
	{
		return;
	}

	if (ECommonInputMode::Game == config->GetInputMode())
	{
		return;
	}

	APlayerController* pc = GetOwningPlayer();
	if (not pc)
	{
		return;
	}

	UWidgetBlueprintLibrary::SetInputMode_GameOnly(pc);
	pc->SetShowMouseCursor(false);
}

TOptional<FUIInputConfig> UBaseActivatableWidget::GetDesiredInputConfig() const
{
	if (_useCustomInputConfig)
	{
		return _inputConfig;
	}
	return TOptional<FUIInputConfig>();
}

#if WITH_EDITOR
void UBaseActivatableWidget::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	ApplyDesiredInputConfig();
}
#endif

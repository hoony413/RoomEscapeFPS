// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InteractionPanel.h"
#include "Components/TextBlock.h"
#include "CommonActionWidget.h"
#include "InputAction.h"

void UInteractionPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UInteractionPanel::SetText(const FText& txt)
{
	if (IsValid(InfoText))
	{
		InfoText->SetText(txt);
	}
}

void UInteractionPanel::SetInputAction(UInputAction* InUseAction)
{
	if (not IsValid(ActionWidget))
	{
		return;
	}

	if (_cachedUseAction == InUseAction)
	{
		return;
	}

	_cachedUseAction = InUseAction;

	if (not IsValid(InUseAction))
	{
		ActionWidget->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	ActionWidget->SetEnhancedInputAction(InUseAction);
	ActionWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}
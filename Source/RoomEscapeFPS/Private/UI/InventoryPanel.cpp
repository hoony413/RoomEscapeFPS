// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventoryPanel.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"

void UInventoryPanel::SetFlashBatteryVisibility(bool bOnOff)
{
	if (FlashProgressBar)
	{
		FlashProgressBar->SetVisibility(bOnOff ? ESlateVisibility::SelfHitTestInvisible :
			ESlateVisibility::Hidden);
	}
}
void UInventoryPanel::SetCharmVisibility(bool bOnOff)
{
	if (CharmCanvasPanel)
	{
		CharmCanvasPanel->SetVisibility(bOnOff ? ESlateVisibility::SelfHitTestInvisible :
			ESlateVisibility::Hidden);
	}
}
void UInventoryPanel::UpdateBatteryPower(float fCurrentBatteryValue)
{
	if (FlashProgressBar)
	{
		FlashProgressBar->SetPercent(fCurrentBatteryValue);
	}
}
void UInventoryPanel::UpdateCharmCount(int32 InCount)
{
	if (CharmCountText)
	{
		FText txt = FText::FromString(FString::Printf(TEXT("x%d"), InCount));
		CharmCountText->SetText(txt);
	}
}

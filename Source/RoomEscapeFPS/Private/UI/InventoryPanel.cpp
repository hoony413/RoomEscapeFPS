// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventoryPanel.h"
#include "Components/ProgressBar.h"

void UInventoryPanel::SetItemToQuickSlot(EItemType InType)
{

}

void UInventoryPanel::SetFlashBatteryVisibility(bool bOnOff)
{
	if (FlashProgressBar)
	{
		FlashProgressBar->SetVisibility(bOnOff ? ESlateVisibility::SelfHitTestInvisible :
			ESlateVisibility::Hidden);
	}
}

void UInventoryPanel::UpdateProgressBar(float fCurrentBatteryValue)
{
	if (FlashProgressBar)
	{
		FlashProgressBar->SetPercent(fCurrentBatteryValue);
	}
}


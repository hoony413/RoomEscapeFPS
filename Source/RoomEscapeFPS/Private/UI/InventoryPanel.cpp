// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventoryPanel.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"

void UInventoryPanel::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	Super::OnAnimationFinished_Implementation(Animation);
	if (Animation == AnimArray[0])
	{
		PlayAnimation(AnimArray[1]);
		UpdateNextInformation(CurrentType, CurrentType, 0);
	}
}

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
void UInventoryPanel::SetCrossHairVisibility(bool bOnOff)
{
	if (CrossHairImage)
	{
		CrossHairImage->SetVisibility(bOnOff ? ESlateVisibility::SelfHitTestInvisible :
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
void UInventoryPanel::UpdateNextInformation(ENextInformationType curType, ENextInformationType nextType, int32 InCount)
{
	TArray<FStringFormatArg> args;
	args.Add(InCount);
	NextInformationText->SetText(FText::FromString(FString::Format(*NextInformationStrArray[(uint8)curType], args)));
	if (CurrentType != nextType)
	{
		CurrentType = nextType;
		PlayAnimation(AnimArray[0]);
	}
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SuccessFailPanel.h"
#include "Components/EditableTextBox.h"

void USuccessFailPanel::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	Super::OnAnimationFinished_Implementation(Animation);
	if (!bReverseAnim && Animation == AnimArray[0])
	{
		bReverseAnim = true;
		PlayAnimation(AnimArray[0], 0.f, 1, EUMGSequencePlayMode::Reverse);
	}
	else if (bReverseAnim)
	{
		RemoveFromParent();
	}
}

void USuccessFailPanel::LaunchAnimation(bool bSuccess)
{
	SetWidgetAnimation();

	if (bSuccess)
	{
		NoticeTextBox->SetText(FText::FromName(FName(TEXT("SUCCESS"))));
	}
	else
	{
		NoticeTextBox->SetText(FText::FromName(FName(TEXT("FAILED"))));
	}

	AddToPlayerScreen();
	bReverseAnim = false;
	PlayAnimation(AnimArray[0]);
}
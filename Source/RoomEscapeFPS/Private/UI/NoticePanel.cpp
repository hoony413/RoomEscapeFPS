// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/NoticePanel.h"
#include "Components/TextBlock.h"

void UNoticePanel::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
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

void UNoticePanel::OpenNotice(ENoticeType InType)
{
	SetWidgetAnimation();

	if (InType == ENoticeType::ESuccess)
	{
		NoticeTextBox->SetText(FText::FromName(FName(TEXT("SUCCESS"))));
	}
	else if(InType == ENoticeType::EFailed)
	{
		NoticeTextBox->SetText(FText::FromName(FName(TEXT("FAILED"))));
	}
	else if (InType == ENoticeType::EOpenNextDoor)
	{
		NoticeTextBox->SetText(FText::FromName(FName(TEXT("DOOR UNLOCKED!"))));
	}
	else if (InType == ENoticeType::EDoorLocked)
	{
		NoticeTextBox->SetText(FText::FromName(FName(TEXT("Door is locked"))));
	}

	AddToPlayerScreen();
	bReverseAnim = false;
	PlayAnimation(AnimArray[0]);
}
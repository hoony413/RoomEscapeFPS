// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/NoticePanel.h"
#include "Components/TextBlock.h"
#include "Helper/Helper.h"
#include "Managers/UISubsystem.h"

void UNoticePanel::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	Super::OnAnimationFinished_Implementation(Animation);
	if (not bReverseAnim && Animation == AnimArray[0])
	{
		bReverseAnim = true;
		PlayAnimation(AnimArray[0], 0.f, 1, EUMGSequencePlayMode::Reverse);
	}
	else if (bReverseAnim)
	{
		UUISubsystem* uiSubsystem = GetGameInstance()->GetSubsystem<UUISubsystem>();
		if (uiSubsystem)
		{
			uiSubsystem->CloseWidget(this);
		}
	}
}

void UNoticePanel::OpenNotice(ENoticeType InType)
{
	if (InType == ENoticeType::SUCCESS)
	{
		NoticeTextBox->SetText(FText::FromName(FName(TEXT("SUCCESS"))));
	}
	else if (InType == ENoticeType::FAILED)
	{
		NoticeTextBox->SetText(FText::FromName(FName(TEXT("FAILED"))));
	}
	else if (InType == ENoticeType::OPEN_NEXT_DOOR)
	{
		NoticeTextBox->SetText(FText::FromName(FName(TEXT("DOOR UNLOCKED!"))));
	}
	else if (InType == ENoticeType::DOOR_LOCKED)
	{
		NoticeTextBox->SetText(FText::FromName(FName(TEXT("Door is locked"))));
	}

	bReverseAnim = false;
	PlayAnimation(AnimArray[0]);
}
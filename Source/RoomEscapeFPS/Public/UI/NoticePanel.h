// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseWidget.h"
#include "NoticePanel.generated.h"

/**
 * 
 */
UENUM()
enum class ENoticeType
{
	ESuccess,
	EFailed,
	EOpenNextDoor,
	EDoorLocked,
};
UCLASS()
class ROOMESCAPEFPS_API UNoticePanel : public UBaseWidget
{
	GENERATED_BODY()
	
public:
	virtual FString GetBPPath() { return TEXT("WidgetBlueprint'/Game/Resources/Widgets/InGame/NoticePanel_Widget.NoticePanel_Widget_C'"); }
	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;

	void OpenNotice(ENoticeType InType);

private:
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* NoticeTextBox;

	bool bReverseAnim = false;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Base/BaseActivatableWidget.h"
#include "NoticePanel.generated.h"

class UTextBlock;
/**
 * 
 */
UENUM()
enum class ENoticeType
{
	NONE = 0 UMETA(Hidden),
	SUCCESS,
	FAILED,
	OPEN_NEXT_DOOR,
	DOOR_LOCKED,
	MAX UMETA(Hidden)
};

UCLASS()
class ROOMESCAPEFPS_API UNoticePanel : public UBaseActivatableWidget
{
	GENERATED_BODY()
	
public:
	void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;

	void OpenNotice(ENoticeType InType);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> NoticeTextBox;

	bool bReverseAnim = false;
};

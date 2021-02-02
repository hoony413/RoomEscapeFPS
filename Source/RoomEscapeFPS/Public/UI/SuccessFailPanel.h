// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseWidget.h"
#include "SuccessFailPanel.generated.h"

/**
 * 
 */
UCLASS()
class ROOMESCAPEFPS_API USuccessFailPanel : public UBaseWidget
{
	GENERATED_BODY()
	
public:
	virtual FString GetBPPath() { return TEXT("WidgetBlueprint'/Game/Resources/Widgets/SuccessFailPanel_Widget.SuccessFailPanel_Widget_C'"); }
	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;

	void LaunchAnimation(bool bSuccess);

private:
	UPROPERTY(meta = (BindWidget))
		class UEditableTextBox* NoticeTextBox;

	bool bReverseAnim = false;
};

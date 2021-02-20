// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseWidget.h"
#include "FirstGetItemInfoPanel.generated.h"

/**
 * 
 */
UCLASS()
class ROOMESCAPEFPS_API UFirstGetItemInfoPanel : public UBaseWidget
{
	GENERATED_BODY()

public:

	virtual void NativeOnInitialized() override;

	virtual FString GetBPPath() override {
		return TEXT(
			"WidgetBlueprint'/Game/Resources/Widgets/InGame/FirstGetItemInfo_Widget.FirstGetItemInfo_Widget_C'");
	}

	void SetItemNameText(const FString& InStr);
	void SetItemDescText(const FString& InStr);

protected:
	UFUNCTION()
		void OnClickedCloseButton();
	
protected:
	UPROPERTY(meta = (BindWidget))
		class UImage* RenderTargetImage;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ItemNameText;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ItemDescText;
	UPROPERTY(meta = (BindWidget))
		class UButton* CloseButton;

};

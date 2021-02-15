// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseWidget.h"
#include "Gameplay/TypeInfoHeader.h"
#include "InventoryPanel.generated.h"

/**
 * 
 */
UCLASS()
class ROOMESCAPEFPS_API UInventoryPanel : public UBaseWidget
{
	GENERATED_BODY()

public:
	virtual FString GetBPPath() override {
		return TEXT(
			"WidgetBlueprint'/Game/Resources/Widgets/InventoryPanel_Widget.InventoryPanel_Widget_C'");
	}

	void SetFlashBatteryVisibility(bool bOnOff);
	void SetCharmVisibility(bool bOnOff);

	void UpdateBatteryPower(float fCurrentBatteryValue);
	void UpdateCharmCount(int32 InCount);

protected:
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* FlashProgressBar;
	UPROPERTY(meta = (BindWidget))
		class UCanvasPanel* CharmCanvasPanel;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* CharmCountText;
};

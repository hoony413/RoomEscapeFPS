// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseActivatableWidget.h"
#include "Gameplay/TypeInfoHeader.h"
#include "InventoryPanel.generated.h"

/**
 * 
 */

class UImage;
class UTextBlock;
class UCanvasPanel;
class UProgressBar;

UCLASS()
class ROOMESCAPEFPS_API UInventoryPanel : public UBaseActivatableWidget
{
	GENERATED_BODY()

public:
	void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;

	void SetFlashBatteryVisibility(bool bOnOff);
	void SetCharmVisibility(bool bOnOff);
	void SetCrossHairVisibility(bool bOnOff);

	void UpdateBatteryPower(float fCurrentBatteryValue);
	void UpdateCharmCount(int32 InCount);
	void UpdateNextInformation(ENextInformationType curType, ENextInformationType nextType, int32 InCount);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> FlashProgressBar;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> CharmCanvasPanel;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CharmCountText;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> CrossHairImage;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> NextInformationText;

	UPROPERTY(EditAnywhere)
	TArray<FString> NextInformationStrArray;

	ENextInformationType CurrentType{};
	int32 CurrentCount{};
};

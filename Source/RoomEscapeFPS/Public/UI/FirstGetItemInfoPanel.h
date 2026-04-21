// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseActivatableWidget.h"
#include "FirstGetItemInfoPanel.generated.h"

class UButton;
class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class ROOMESCAPEFPS_API UFirstGetItemInfoPanel : public UBaseActivatableWidget
{
	GENERATED_BODY()

public:

	void NativeOnInitialized() override;

	void SetItemNameText(FText const& InText);
	void SetItemDescText(FText const& InText);

protected:
	UFUNCTION()
		void OnClickedCloseButton();
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> RenderTargetImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemNameText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemDescText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CloseButton;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Base/BaseWidget.h"
#include "InteractionPanel.generated.h"

class UImage;
class UTextBlock;

/**
 * C++ Code(UInteractionPanel)
 */
UCLASS()
class ROOMESCAPEFPS_API UInteractionPanel : public UBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeOnInitialized() override;
	void SetText(const FText& txt);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> BackgroundImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> InfoText;

};

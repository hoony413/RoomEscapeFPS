// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseWidget.h"
#include "InteractionPanel.generated.h"

/**
 * C++ Code(UInteractionPanel)
 */
UCLASS()
class ROOMESCAPEFPS_API UInteractionPanel : public UBaseWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;
	virtual FString GetBPPath() override { return TEXT(
"WidgetBlueprint'/Game/Resources/Widgets/InteractionPanel_Widget.InteractionPanel_Widget_C'"); }
	void SetText(const FText& txt);

protected:
	UPROPERTY(meta = (BindWidget))
		class UImage* BackgroundImage;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* InfoText;

};

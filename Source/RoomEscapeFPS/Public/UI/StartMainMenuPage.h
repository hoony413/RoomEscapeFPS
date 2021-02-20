// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseWidget.h"
#include "StartMainMenuPage.generated.h"

/**
 * 
 */
UCLASS()
class ROOMESCAPEFPS_API UStartMainMenuPage : public UBaseWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;
	virtual FString GetBPPath() override { return TEXT("WidgetBlueprint'/Game/Resources/Widgets/OutGame/StartMainMenu_Widget.StartMainMenu_Widget_C'"); }

protected:
	UFUNCTION()
		void OnClickedStartButton();
	UFUNCTION()
		void OnClickedOptionButton();
	UFUNCTION()
		void OnClickedExitButton();

private:
	UPROPERTY(meta = (BindWidget))
		class UButton* StartButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* OptionButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* ExitButton;
};

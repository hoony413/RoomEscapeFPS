// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseWidget.h"
#include "StartMainMenuPage.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class ROOMESCAPEFPS_API UStartMainMenuPage : public UBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeOnInitialized() override;

protected:
	UFUNCTION()
		void OnClickedStartButton();
	UFUNCTION()
		void OnClickedOptionButton();
	UFUNCTION()
		void OnClickedExitButton();

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> StartButton;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> OptionButton;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ExitButton;
};

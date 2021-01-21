// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseWidget.generated.h"

/**
 * 
 */

UCLASS()
class ROOMESCAPEFPS_API UBaseWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	virtual FString GetBPPath() { return ""; }

protected:
	UPROPERTY(EditAnywhere, Category = "ZOrder")
		uint32 ZOrder;

};

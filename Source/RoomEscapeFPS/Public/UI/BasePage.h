// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseWidget.h"
#include "BasePage.generated.h"

/**
 * 
 */
UCLASS()
class ROOMESCAPEFPS_API UBasePage : public UBaseWidget
{
	GENERATED_BODY()

public:
	virtual FString GetBPPath() { return TEXT("WidgetBlueprint'/Game/Resources/Widgets/BaseHUD_Widget.BaseHUD_Widget_C'"); }
	
};

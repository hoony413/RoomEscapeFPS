// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseWidget.h"
#include "LoadingScreen.generated.h"

/**
 * 
 */
UCLASS()
class ROOMESCAPEFPS_API ULoadingScreen : public UBaseWidget
{
	GENERATED_BODY()
	
public:
	virtual FString GetBPPath() { return TEXT("WidgetBlueprint'/Game/Resources/Widgets/OutGame/Loading_Widget.Loading_Widget_C'"); }
};

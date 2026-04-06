// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Base/BaseWidget.h"

void UBaseWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	SetWidgetAnimation();
}

#if WITH_EDITOR
void UBaseWidget::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

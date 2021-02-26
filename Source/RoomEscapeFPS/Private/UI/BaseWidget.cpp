// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BaseWidget.h"
#define VariableNameToTEXT(name) FString::Printf(TEXT("%s"), name);

void UBaseWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	SetWidgetAnimation();
}

void UBaseWidget::SetWidgetAnimation()
{
	AnimArray.Empty();
	UWidgetBlueprintGeneratedClass* WidgetClass = GetWidgetTreeOwningClass();
	for (int32 i = 0; i < WidgetClass->Animations.Num(); i++) {

		AnimArray.Add(WidgetClass->Animations[i]);
	}
}

#if WITH_EDITOR
void UBaseWidget::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	//if (PropertyChangedEvent.MemberProperty)
	//{
	//	FName propertyName = PropertyChangedEvent.MemberProperty->GetFName();
	//	if (propertyName == GET_MEMBER_NAME_CHECKED(UBaseWidget, Images))
	//	{
	//	}
	//	else if (propertyName == GET_MEMBER_NAME_CHECKED(UBaseWidget, TextBlocks))
	//	{
	//	}
	//}
}
#endif
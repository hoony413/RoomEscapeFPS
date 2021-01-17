// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BaseWidget.h"
#define VariableNameToTEXT(name) FString::Printf(TEXT("%s"), name);

void UBaseWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
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
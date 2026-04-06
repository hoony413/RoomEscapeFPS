// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintGeneratedClass.h"
#include "Animation/WidgetAnimation.h"
#include "WidgetAnimationInterface.generated.h"

UINTERFACE()
class ROOMESCAPEFPS_API UWidgetAnimationInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 위젯 애니메이션 배열을 관리하는 인터페이스.
 * AnimArray는 GC 보호를 위해 각 구현 클래스의 UPROPERTY로 유지하고,
 * GetAnimationArray()로 접근하여 SetWidgetAnimation()의 공통 로직을 공유한다.
 */
class IWidgetAnimationInterface
{
	GENERATED_BODY()

protected:
	virtual TArray<TObjectPtr<UWidgetAnimation>>& GetAnimationArray() = 0;

	virtual void SetWidgetAnimation()
	{
		TArray<TObjectPtr<UWidgetAnimation>>& animArray = GetAnimationArray();
		animArray.Empty();
		if (UUserWidget* widget = Cast<UUserWidget>(this))
		{
			UWidgetBlueprintGeneratedClass* widgetClass = widget->GetWidgetTreeOwningClass();
			for (auto const& anim : widgetClass->Animations)
			{
				animArray.Add(anim);
			}
		}
	}
};

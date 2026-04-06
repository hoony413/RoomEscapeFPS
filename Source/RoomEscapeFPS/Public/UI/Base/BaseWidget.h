// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Interface/View/ViewModelInterface.h"
#include "Interface/WidgetAnimationInterface.h"
#include "BaseWidget.generated.h"

/**
 *
 */

UCLASS(Abstract)
class ROOMESCAPEFPS_API UBaseWidget
	: public UCommonUserWidget
	, public IWidgetAnimationInterface
	, public IViewModelOwnerInterface
{
	GENERATED_BODY()

public:
	void NativeOnInitialized() override;

#if WITH_EDITOR
	void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:
	// IViewModelOwnerInterface - 서브클래스에서 선택적으로 오버라이드
	void SetViewModel() override {}

protected:
	TArray<TObjectPtr<UWidgetAnimation>>& GetAnimationArray() override { return AnimArray; }

protected:
	UPROPERTY(Transient)
TArray<TObjectPtr<UWidgetAnimation>> AnimArray;

};

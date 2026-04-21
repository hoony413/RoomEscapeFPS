// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "UI/Base/BaseWidget.h"
#include "ActionDescWidget.generated.h"

class UInputAction;
class UCommonTextBlock;
class UCommonActionWidget;

UCLASS(meta = (DisableNativeTick))
class ROOMESCAPEFPS_API UActionDescWidget : public UBaseWidget
{
	GENERATED_BODY()
	
protected:
	void NativeConstruct() override;

private:
	UPROPERTY(Transient, meta = (BindWidget))
	TObjectPtr<UCommonActionWidget> ActionWidget;

	UPROPERTY(Transient, meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> ActionTextBlock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> BindInputAction;
};

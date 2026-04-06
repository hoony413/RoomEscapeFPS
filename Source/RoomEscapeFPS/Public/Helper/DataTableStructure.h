// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataTableStructure.generated.h"

UENUM(BlueprintType)
enum class EActivatableWidgetType : uint8
{
	NONE = 0 UMETA(Hidden),
	INTERACTION,
	NOTICE,
	PIPE_GAME,
	FIRST_GET_ITEM_INFO,
	MAX UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EActivatableWidgetLayerType : uint8
{
	NONE = 0 UMETA(Hidden),
	FULL_SCREEN,
	POPUP,
	MAX UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FActivatableWidgetInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
EActivatableWidgetType WidgetType{};

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
EActivatableWidgetLayerType LayerType{};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
TSoftClassPtr<UUserWidget> WidgetClass;
};

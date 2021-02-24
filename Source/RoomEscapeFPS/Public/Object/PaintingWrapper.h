// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/SolutionWrapper.h"
#include "PaintingWrapper.generated.h"

/**
 * 
 */
UCLASS()
class ROOMESCAPEFPS_API APaintingWrapper : public ASolutionWrapper
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};

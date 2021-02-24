// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/SolutionWrapper.h"
#include "SwitchButtonWrapper.generated.h"

/**
 * 
 */
UCLASS()
class ROOMESCAPEFPS_API ASwitchButtonWrapper : public ASolutionWrapper
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};

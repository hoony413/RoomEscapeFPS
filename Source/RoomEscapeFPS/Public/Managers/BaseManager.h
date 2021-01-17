// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BaseManager.generated.h"

/**
 * 
 */
UCLASS()
class ROOMESCAPEFPS_API UBaseManager : public UObject
{
	GENERATED_BODY()
	
public:
	virtual void InitManager();
	virtual void ReleaseManager();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/GetableObject.h"
#include "ClueObject.generated.h"

/**
 * 
 */
UCLASS()
class ROOMESCAPEFPS_API AClueObject : public AGetableObject
{
	GENERATED_BODY()
	
public:
	AClueObject();

	virtual void OnInteraction(class APawn* requester, class UPrimitiveComponent* InComp) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};

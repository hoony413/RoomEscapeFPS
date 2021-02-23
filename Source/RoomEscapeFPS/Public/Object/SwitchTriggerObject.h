// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/InteractiveObject.h"
#include "SwitchTriggerObject.generated.h"

/**
 * 
 */
UCLASS()
class ROOMESCAPEFPS_API ASwitchTriggerObject : public AInteractiveObject
{
	GENERATED_BODY()
	
public:
	virtual void OnInteraction(APawn* requester, class UPrimitiveComponent* InComp) override;
	virtual void NetMulticast_Interaction_Implementation(int32 index, EInteractiveObjectState InState) override;

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	EInteractiveObjectState cachedState;
};

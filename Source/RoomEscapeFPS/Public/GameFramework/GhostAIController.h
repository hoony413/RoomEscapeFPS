// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GhostAIController.generated.h"

/**
 * 
 */

UENUM()
enum class EGhostStateMachine
{
	NONE = 0 UMETA(Hidden),
	IDLE,
	MOVE_TO,
	MOVE_COMPLETE,
	DEAD,
	MAX UMETA(Hidden)
};
UCLASS()
class ROOMESCAPEFPS_API AGhostAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AGhostAIController();

	void Tick(float DeltaTime) override;

	FORCEINLINE EGhostStateMachine GetGhostState() { return CurrentState; }
	void SetGhostState(EGhostStateMachine InState);

protected:
	void BeginPlay() override;
		
	void MoveToGhostInBoundingBoxRandomPos();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastOnGhostDead();

private:
	FVector cachedMoveDirection;
	FVector MoveFromPos;
	FVector MoveToPos;

	float fLimitDelta = 2.f;
	float fDelta = 0.f;
	bool bActive;
	
	UPROPERTY()
	EGhostStateMachine CurrentState;
};

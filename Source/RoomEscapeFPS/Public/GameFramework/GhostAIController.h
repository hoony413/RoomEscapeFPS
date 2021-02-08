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
	EIdle,
	EMoveTo,
	EMoveComplete,
	EDead,
};
UCLASS()
class ROOMESCAPEFPS_API AGhostAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AGhostAIController();

	virtual void Tick(float DeltaTime) override;

	void SetGhostState(EGhostStateMachine InState);

protected:
	virtual void BeginPlay() override;
		
	void MoveToGhostInBoundingBoxRandomPos();

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

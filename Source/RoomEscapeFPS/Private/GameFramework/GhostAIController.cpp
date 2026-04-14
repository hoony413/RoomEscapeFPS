// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/GhostAIController.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/RoomEscapeFPSGameState.h"
#include "Helper/Helper.h"
#include "Object/GhostSoul.h"
#include "Gameplay/GhostSpawner.h"

AGhostAIController::AGhostAIController()
{
	//AttachToComponent(RootComponent, 
	//	FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
}

void AGhostAIController::BeginPlay()
{
	Super::BeginPlay();
	SetGhostState(EGhostStateMachine::IDLE);
	bActive = true;
	fDelta = 0.f;
}

void AGhostAIController::SetGhostState(EGhostStateMachine InState)
{
	CurrentState = InState;
	if (CurrentState == EGhostStateMachine::IDLE)
	{
		bActive = true;
		fDelta = 0.f;
	}
	else if (CurrentState == EGhostStateMachine::DEAD)
	{
		AGhostSoul* ghost = Cast<AGhostSoul>(GetPawn());
		if (ghost)
		{
			bActive = false;
			fDelta = 0.f;
			ARoomEscapeFPSGameState* gs = Helper::GetGameState(GetWorld());
			if (gs)
			{
				gs->IncreaseGhostDeadCount();
			}
			NetMulticastOnGhostDead();
		}
	}
}

void AGhostAIController::MoveToGhostInBoundingBoxRandomPos()
{
	APawn* pawn = GetPawn();
	if (pawn)
	{
		AGhostSoul* ghost = Cast<AGhostSoul>(pawn);
		if (ghost)
		{
			UBoxComponent* box = ghost->GetBoundingBox();
			if (box)
			{
				CurrentState = EGhostStateMachine::MOVE_TO;
				MoveFromPos = GetPawn()->GetActorLocation();
				MoveToPos = UKismetMathLibrary::RandomPointInBoundingBox(
					box->Bounds.Origin,
					box->Bounds.BoxExtent);

				cachedMoveDirection = MoveToPos - MoveFromPos;
				cachedMoveDirection.Normalize();
			}
		}
	}
}
void AGhostAIController::NetMulticastOnGhostDead_Implementation()
{
	AGhostSoul* ghost = Cast<AGhostSoul>(GetPawn());
	if (not ghost)
	{
		return;
	}

	if (IsNetMode(NM_DedicatedServer))
	{
		ghost->Destroy();
	}
}

void AGhostAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (not bActive)
	{
		return;
	}
	
	if (CurrentState == EGhostStateMachine::IDLE)
	{
		MoveToGhostInBoundingBoxRandomPos();
	}
	else if (CurrentState == EGhostStateMachine::MOVE_TO)
	{
		fDelta += DeltaTime;
		APawn* pawn = GetPawn();
		if (pawn)
		{
			pawn->AddMovementInput(cachedMoveDirection, 1.f, true);
			if (FVector::Dist(pawn->GetActorLocation(), MoveToPos) < 100.f)
			{
				CurrentState = EGhostStateMachine::MOVE_COMPLETE;
			}
			else if (fDelta > fLimitDelta)
			{
				CurrentState = EGhostStateMachine::MOVE_COMPLETE;
			}
		}
	}
	else if (CurrentState == EGhostStateMachine::MOVE_COMPLETE)
	{
		fDelta = 0.f;
		CurrentState = EGhostStateMachine::IDLE;
	}
}
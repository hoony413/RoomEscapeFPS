// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/GhostAIController.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Object/GhostSoul.h"

AGhostAIController::AGhostAIController()
{
	//AttachToComponent(RootComponent, 
	//	FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
}

void AGhostAIController::BeginPlay()
{
	Super::BeginPlay();
	//SetGhostState(EGhostStateMachine::EIdle);
	bActive = true;
}

void AGhostAIController::SetGhostState(EGhostStateMachine InState)
{
	CurrentState = InState;
	if (CurrentState == EGhostStateMachine::EIdle)
	{
		bActive = true;
		fDelta = 0.f;
	}
	else if (CurrentState == EGhostStateMachine::EDead)
	{
		APawn* pawn = GetPawn();
		if (pawn)
		{	// TODO: 모든 클라이언트에게 Ghost Disappear 통지.
			bActive = false;
			fDelta = 0.f;
			NetMulticastOnGhostDead();
		}
	}
}

void AGhostAIController::NetMulticastOnGhostDead_Implementation()
{
	APawn* pawn = GetPawn();
	if (pawn)
	{
		AGhostSoul* ghost = Cast<AGhostSoul>(pawn);
		if (ghost)
		{	// 사라짐 애니메이션.
			ghost->SetAsDead();
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
				CurrentState = EGhostStateMachine::EMoveTo;
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
void AGhostAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bActive == false)
		return;

	if (CurrentState == EGhostStateMachine::EIdle)
	{
		MoveToGhostInBoundingBoxRandomPos();
	}
	else if (CurrentState == EGhostStateMachine::EMoveTo)
	{
		fDelta += DeltaTime;
		APawn* pawn = GetPawn();
		if (pawn)
		{
			pawn->AddMovementInput(cachedMoveDirection, 1.f, true);
			if (FVector::Dist(pawn->GetActorLocation(), MoveToPos) < 100.f)
			{
				CurrentState = EGhostStateMachine::EMoveComplete;
			}
			else if (fDelta > fLimitDelta)
			{
				CurrentState = EGhostStateMachine::EMoveComplete;
			}
		}
	}
	else if (CurrentState == EGhostStateMachine::EMoveComplete)
	{
		fDelta = 0.f;
		CurrentState = EGhostStateMachine::EIdle;
	}
}
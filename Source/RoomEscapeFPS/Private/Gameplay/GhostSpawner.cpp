// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/GhostSpawner.h"
#include "Object/GhostSoul.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
#include "GameFramework/GhostAIController.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AGhostSpawner::AGhostSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	SpawnVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnVolume"));
	SetRootComponent(SpawnVolume);
}

// Called when the game starts or when spawned
void AGhostSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (IsNetMode(NM_DedicatedServer))
	{
		fSpawnTime = 1.5f;
		SetActive(false);
	}
}

void AGhostSpawner::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(AGhostSpawner, fSpawnTime, COND_InitialOnly);
	DOREPLIFETIME(AGhostSpawner, bActive);
}

void AGhostSpawner::SpawnGhost()
{
	if (IsNetMode(NM_DedicatedServer) && bActive)
	{
		FActorSpawnParameters params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
		AGhostSoul* ghost = GetWorld()->SpawnActor<AGhostSoul>(GhostClass.LoadSynchronous(), params);
		if (ghost)
		{
			ghost->SetActorLocation(UKismetMathLibrary::RandomPointInBoundingBox(
				SpawnVolume->Bounds.Origin, SpawnVolume->Bounds.BoxExtent));
			ActiveGhosts.Add(ghost);
		}

		float fTime = FMath::RandRange(fSpawnTime - 1.f, fSpawnTime + 1.f);
		GetWorld()->GetTimerManager().SetTimer(SpawnTimer, this, &AGhostSpawner::SpawnGhost,
			fTime, false);
	}
}

void AGhostSpawner::SetActive(bool bInActive)
{
	bActive = bInActive;
	if (IsNetMode(NM_DedicatedServer))
	{
		if (bActive)
		{
			SpawnGhost();
		}
		else
		{
			GetWorld()->GetTimerManager().ClearTimer(SpawnTimer);
			for (AGhostSoul* ghost : ActiveGhosts)
			{
				if (IsValid(ghost))
				{
					ghost->Destroy();
				}
			}
			ActiveGhosts.Empty();
		}
	}
}
// Called every frame
void AGhostSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

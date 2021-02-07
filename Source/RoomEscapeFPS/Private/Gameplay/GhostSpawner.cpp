// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/GhostSpawner.h"
#include "Gameplay/Freelist.h"
#include "Object/GhostSoul.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/BoxComponent.h"

// Sets default values
AGhostSpawner::AGhostSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	SpawnVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnVolume"));
	SetRootComponent(SpawnVolume);

	GhostActorFreelist = CreateDefaultSubobject<UFreelist>(TEXT("GhostFreelist"));
	//GhostActorFreelist->SetNetAddressable();
	//GhostActorFreelist->SetIsReplicated(true);
}

// Called when the game starts or when spawned
void AGhostSpawner::BeginPlay()
{
	Super::BeginPlay();
	bActive = false;
}

// Called every frame
void AGhostSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetNetMode() == NM_DedicatedServer && bActive)
	{
		fSumDelta += DeltaTime;
		if (fSumDelta > fSpawnTime)
		{
			fSumDelta = 0.f;
			AGhostSoul* ghost = GhostActorFreelist->GetElement<AGhostSoul>();

			// TODO: actor의 위치 랜덤 설정 및 이동 처리.
			ghost->SetActorLocation(UKismetMathLibrary::RandomPointInBoundingBox(SpawnVolume->Bounds.Origin, SpawnVolume->Bounds.BoxExtent));
		}
	}
}


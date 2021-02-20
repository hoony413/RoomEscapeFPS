// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/GhostSpawner.h"
#include "Gameplay/Freelist.h"
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

	GhostActorFreelist = CreateDefaultSubobject<UFreelist>(TEXT("GhostFreelist"));
}

// Called when the game starts or when spawned
void AGhostSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (GetNetMode() == NM_DedicatedServer)
	{
		fSpawnTime = 1.5f;
		SetActive(false);
	}
}

void AGhostSpawner::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AGhostSpawner, fSpawnTime);
}

void AGhostSpawner::SpawnGhost()
{
	if (GetNetMode() == NM_DedicatedServer && bActive)
	{
		AGhostSoul* ghost = GhostActorFreelist->GetElement<AGhostSoul>();

		// TODO: actor의 위치 랜덤 설정 및 이동 처리.
		ghost->SetActorLocation(UKismetMathLibrary::RandomPointInBoundingBox(
			SpawnVolume->Bounds.Origin, SpawnVolume->Bounds.BoxExtent));

		float fTime = FMath::RandRange(fSpawnTime - 1.f, fSpawnTime + 1.f);
		GetWorld()->GetTimerManager().SetTimer(SpawnTimer, this, &AGhostSpawner::SpawnGhost,
			fTime, false);
	}
}
void AGhostSpawner::DeactiveGhost(AGhostSoul* ghost)
{
	// TODO: Freelist에 오브젝트를 반납.
	GhostActorFreelist->ReturnElement(ghost);
	//GetWorld()->GetTimerManager().SetTimer(SpawnTimer, this, &AGhostSpawner::SpawnGhost,
	//	1.5f, false);
}
void AGhostSpawner::SetActive(bool bInActive)
{
	bActive = bInActive;
	if (GetNetMode() == NM_DedicatedServer && bActive)
	{
		SpawnGhost();
	}
}
// Called every frame
void AGhostSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

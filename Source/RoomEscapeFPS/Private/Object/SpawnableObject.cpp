// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/SpawnableObject.h"
#include "Object/GetableObject.h"

const FName FlashSpawnerTag = FName(TEXT("FlashSpawner"));
const FName BatterySpawnerTag = FName(TEXT("BatterySpawner"));
const FName CharmSpawnerTag = FName(TEXT("CharmSpawner"));
const FName KeySpawnerTag = FName(TEXT("KeySpawner"));

void ASpawnableObject::BeginPlay()
{
	Super::BeginPlay();

	if (GetNetMode() == NM_DedicatedServer)
	{
		TArray<UStaticMeshComponent*> meshes;
		GetComponents<UStaticMeshComponent>(meshes);
		AGetableObject* obj = nullptr;
		for (auto& elem : meshes)
		{
			elem->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
			if (elem->ComponentHasTag(FlashSpawnerTag))
			{
				obj = GetWorld()->SpawnActor<AGetableObject>(FlashObj.LoadSynchronous(), FActorSpawnParameters());
			}
			else if (elem->ComponentHasTag(BatterySpawnerTag))
			{
				obj = GetWorld()->SpawnActor<AGetableObject>(BatteryObj.LoadSynchronous(), FActorSpawnParameters());
			}
			else if (elem->ComponentHasTag(CharmSpawnerTag))
			{
				obj = GetWorld()->SpawnActor<AGetableObject>(CharmObj.LoadSynchronous(), FActorSpawnParameters());
			}
			else if (elem->ComponentHasTag(KeySpawnerTag))
			{
				obj = GetWorld()->SpawnActor<AGetableObject>(KeyObj.LoadSynchronous(), FActorSpawnParameters());
			}

			if (obj)
			{
				obj->AttachToComponent(elem, FAttachmentTransformRules::KeepWorldTransform);
				obj->SetActorRelativeLocation(FVector(0, 0, 5));
			}
		}
	}
}

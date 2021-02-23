// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/SwitchTriggerObject.h"
#include "Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"

void ASwitchTriggerObject::OnInteraction(APawn* requester, class UPrimitiveComponent* InComp)
{
	Super::OnInteraction(requester, InComp);
}

void ASwitchTriggerObject::NetMulticast_Interaction_Implementation(int32 index, EInteractiveObjectState InState)
{
	Super::NetMulticast_Interaction_Implementation(index, InState);

	float emValue = TimelineMeshes[0].CurrentState == EInteractiveObjectState::EState_Close_Or_Off ? 0 : 64;

	UMaterialInterface* mt = TimelineMeshes[0].StaticMeshComponent->GetMaterial(0);
	UMaterialInstanceDynamic* DynMaterial = TimelineMeshes[0].StaticMeshComponent->CreateDynamicMaterialInstance(0, mt);
	if (DynMaterial)
	{
		DynMaterial->SetScalarParameterValue(TEXT("Emissive"), emValue);
	}
}

void ASwitchTriggerObject::BeginPlay()
{
	Super::BeginPlay();
	
}
void ASwitchTriggerObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float emValue = TimelineMeshes[0].CurrentState == EInteractiveObjectState::EState_Close_Or_Off ? 0 : 64;
	UMaterialInterface* mt = TimelineMeshes[0].StaticMeshComponent->GetMaterial(0);
	UMaterialInstanceDynamic* DynMaterial = TimelineMeshes[0].StaticMeshComponent->CreateDynamicMaterialInstance(0, mt);
	if (DynMaterial)
	{
		DynMaterial->SetScalarParameterValue(TEXT("Emissive"), emValue);
	}
}
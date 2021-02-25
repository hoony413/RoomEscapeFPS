// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/DynamicMtInteractiveObject.h"
#include "Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"

bool ADynamicMtInteractiveObject::OnInteraction(APawn* requester, class UPrimitiveComponent* InComp)
{
	if (Super::OnInteraction(requester, InComp) == false)
		return false;

	int32 index = 0;
	int32 NotUseDummy = 0;
	FDynamicMtInfo* find = FindDynamicMtMeshComponent(Cast<UStaticMeshComponent>(InComp), index);
	FTimelineInfo* tFind = FindTimelineMeshComponent(Cast<UStaticMeshComponent>(InComp), NotUseDummy);
	NetMulticast_DynamicMaterial(index, tFind->CurrentState);
	return true;
}
void ADynamicMtInteractiveObject::NetMulticast_DynamicMaterial_Implementation(int32 index, EInteractiveObjectState InState)
{
	if (GetNetMode() == NM_Client)
	{
		if (DynamicMtArray[index].DynamicMt)
		{
			if (DynamicMtArray[index].ParameterType == EDynamicMtParamType::EScalar)
			{
				DynamicMtArray[index].DynamicMt->SetScalarParameterValue(DynamicMtArray[index].ParameterName, 
					InState == EInteractiveObjectState::EState_Close_Or_Off ? DynamicMtArray[index].ParameterValue.Min : DynamicMtArray[index].ParameterValue.Max);
			}
			else if (DynamicMtArray[index].ParameterType == EDynamicMtParamType::EVector)
			{
				DynamicMtArray[index].DynamicMt->SetVectorParameterValue(DynamicMtArray[index].ParameterName, 
					InState == EInteractiveObjectState::EState_Close_Or_Off ? DynamicMtArray[index].ParameterVector.v1 : DynamicMtArray[index].ParameterVector.v2);
			}
		}
	}
}
FDynamicMtInfo* ADynamicMtInteractiveObject::FindDynamicMtMeshComponent(class UStaticMeshComponent* InMesh, int32& OutIndex)
{
	for (int32 i = 0; i < DynamicMtArray.Num(); ++i)
	{
		if (InMesh == DynamicMtArray[i].StaticMeshComponent)
		{
			OutIndex = i;
			return &DynamicMtArray[i];
		}
	}
	return nullptr;
}

const FName DynamicMtTag = FName(TEXT("DynamicMt"));
void ADynamicMtInteractiveObject::BeginPlay()
{
	Super::BeginPlay();
	if (GetNetMode() == NM_Client)
	{
		TArray<UStaticMeshComponent*> meshes;
		GetComponents<UStaticMeshComponent>(meshes);
		for (int32 i = 0, j = 0; i < meshes.Num(); ++i)
		{
			if (meshes[i]->ComponentHasTag(DynamicMtTag))
			{
				UMaterialInterface* mt = meshes[i]->GetMaterial(0);
				UMaterialInstanceDynamic* dm = meshes[i]->CreateDynamicMaterialInstance(0, mt);
				
				DynamicMtArray[j].StaticMeshComponent = meshes[i];
				DynamicMtArray[j].DynamicMt = dm;
				++j;
			}
		}
	}
}


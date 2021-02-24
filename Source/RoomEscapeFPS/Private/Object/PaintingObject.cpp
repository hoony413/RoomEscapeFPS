// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/PaintingObject.h"
#include "Net/UnrealNetwork.h"

APaintingObject::APaintingObject()
{
	RotateState = ERotateState::ERotate_0;
}

void APaintingObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APaintingObject, RotateState);
}
void APaintingObject::OnInteraction(APawn* requester, class UPrimitiveComponent* InComp)
{
	if (GetNetMode() == NM_DedicatedServer)
	{
		if (RotateState == ERotateState::ERotate_270)
		{
			RotateState = ERotateState::ERotate_0;
		}
		else
		{
			RotateState = (ERotateState)((uint8)RotateState + 1u);
		}
	}
	Super::OnInteraction(requester, InComp);
}

void APaintingObject::NetMulticast_Timeline_Implementation(int32 index, EInteractiveObjectState InState)
{
	switch (TimelineMeshes[index].ControlType)
	{
	case ETimelineControlType::ELocationX:
		StartCurveValue =
			TimelineMeshes[index].StaticMeshComponent->GetRelativeLocation().X;
		break;
	case ETimelineControlType::ELocationY:
		StartCurveValue =
			TimelineMeshes[index].StaticMeshComponent->GetRelativeLocation().Y;
		break;
	case ETimelineControlType::ELocationZ:
		StartCurveValue =
			TimelineMeshes[index].StaticMeshComponent->GetRelativeLocation().Z;
		break;
	case ETimelineControlType::ERotationX:
		StartCurveValue =
			TimelineMeshes[index].StaticMeshComponent->GetRelativeRotation().Roll;
		break;
	case ETimelineControlType::ERotationY:
		StartCurveValue =
			TimelineMeshes[index].StaticMeshComponent->GetRelativeRotation().Yaw;
		break;
	case ETimelineControlType::ERotationZ:
		StartCurveValue =
			TimelineMeshes[index].StaticMeshComponent->GetRelativeRotation().Pitch;
		break;
	}
	TimelineMeshes[index].Timeline.PlayFromStart();
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/PaintingObject.h"
#include "Net/UnrealNetwork.h"

APaintingObject::APaintingObject()
{
	RotateState = ERotateState::ROTATE_0;
}

void APaintingObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APaintingObject, RotateState);
	DOREPLIFETIME(APaintingObject, Digit);
}
bool APaintingObject::OnInteraction(APawn* requester, class UPrimitiveComponent* InComp)
{
	ERotateState cachedRotateState = RotateState;
	if (IsNetMode(NM_DedicatedServer))
	{
		if (RotateState == ERotateState::ROTATE_270)
		{
			RotateState = ERotateState::ROTATE_0;
		}
		else
		{
			RotateState = (ERotateState)((uint8)RotateState + 1u);
		}
	}

	if (Super::OnInteraction(requester, InComp) == false)
	{
		if (IsNetMode(NM_DedicatedServer))
		{	// 백업된 로테이트 값으로 다시 복구.
			RotateState = cachedRotateState;
		}
		return false;
	}
	
	return true;
}

void APaintingObject::NetMulticast_Timeline_Implementation(int32 index, EInteractiveObjectState InState)
{
	//switch (TimelineMeshes[index].ControlType)
	//{
	//case ETimelineControlType::ROTATION_X:
	//	StartCurveValue =
	//		TimelineMeshes[index].StaticMeshComponent->GetRelativeRotation().Roll;
	//	break;
	//case ETimelineControlType::ROTATION_Y:
	//	StartCurveValue =
	//		TimelineMeshes[index].StaticMeshComponent->GetRelativeRotation().Yaw;
	//	break;
	//case ETimelineControlType::ROTATION_Z:
	//	StartCurveValue =
	//		TimelineMeshes[index].StaticMeshComponent->GetRelativeRotation().Pitch;
	//	break;
	//}
	// 이미 회전 처리가 되었기 때문에, 회전 이전 초기값을 구하려면 -1 해야 한다.
	StartCurveValue = (((uint8)RotateState - 1u) * 90) % 360;
	TimelineMeshes[index].Timeline.PlayFromStart();
}

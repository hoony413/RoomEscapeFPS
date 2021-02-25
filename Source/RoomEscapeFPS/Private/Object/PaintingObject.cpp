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
	DOREPLIFETIME(APaintingObject, Digit);
}
bool APaintingObject::OnInteraction(APawn* requester, class UPrimitiveComponent* InComp)
{
	ERotateState cachedRotateState = RotateState;
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

	if (Super::OnInteraction(requester, InComp) == false)
	{
		if (GetNetMode() == NM_DedicatedServer)
		{	// ����� ������Ʈ ������ �ٽ� ����.
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
	//case ETimelineControlType::ERotationX:
	//	StartCurveValue =
	//		TimelineMeshes[index].StaticMeshComponent->GetRelativeRotation().Roll;
	//	break;
	//case ETimelineControlType::ERotationY:
	//	StartCurveValue =
	//		TimelineMeshes[index].StaticMeshComponent->GetRelativeRotation().Yaw;
	//	break;
	//case ETimelineControlType::ERotationZ:
	//	StartCurveValue =
	//		TimelineMeshes[index].StaticMeshComponent->GetRelativeRotation().Pitch;
	//	break;
	//}
	// �̹� ȸ�� ó���� �Ǿ��� ������, ȸ�� ���� �ʱⰪ�� ���Ϸ��� -1 �ؾ� �Ѵ�.
	StartCurveValue = (((uint8)RotateState - 1u) * 90) % 360;
	TimelineMeshes[index].Timeline.PlayFromStart();
}

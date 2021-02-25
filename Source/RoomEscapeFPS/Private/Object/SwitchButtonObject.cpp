// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/SwitchButtonObject.h"
#include "Net/UnrealNetwork.h"

ASwitchButtonObject::ASwitchButtonObject()
{
	bSwitchPressed = false;
}
void ASwitchButtonObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASwitchButtonObject, bSwitchPressed);
	DOREPLIFETIME(ASwitchButtonObject, Digit);
}
bool ASwitchButtonObject::OnInteraction(APawn* requester, class UPrimitiveComponent* InComp)
{
	if (GetNetMode() == NM_DedicatedServer)
	{
		bSwitchPressed = !bSwitchPressed;
	}
	
	if (Super::OnInteraction(requester, InComp) == false)
	{	// 스위치 변경 후, 부모 딴에서 false 리턴된 경우 플래그값을 초기 상태로 다시 되돌린다.
		// TODO: 델리게이트가 부모 클래스에 있어 부득이 이런 구조로 되었는데... 개선방법 고민
		if (GetNetMode() == NM_DedicatedServer)
		{
			bSwitchPressed = !bSwitchPressed;
		}
		return false;
	}
	return true;
}
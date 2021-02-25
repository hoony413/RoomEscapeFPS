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
	{	// ����ġ ���� ��, �θ� ������ false ���ϵ� ��� �÷��װ��� �ʱ� ���·� �ٽ� �ǵ�����.
		// TODO: ��������Ʈ�� �θ� Ŭ������ �־� �ε��� �̷� ������ �Ǿ��µ�... ������� ���
		if (GetNetMode() == NM_DedicatedServer)
		{
			bSwitchPressed = !bSwitchPressed;
		}
		return false;
	}
	return true;
}
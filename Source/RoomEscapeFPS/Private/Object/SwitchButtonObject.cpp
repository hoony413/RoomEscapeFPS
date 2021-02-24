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
void ASwitchButtonObject::OnInteraction(APawn* requester, class UPrimitiveComponent* InComp)
{
	if (GetNetMode() == NM_DedicatedServer)
	{
		bSwitchPressed = !bSwitchPressed;
	}
	Super::OnInteraction(requester, InComp);
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/TriggerObject.h"
#include "GameFramework/RoomEscapeFPSGameMode.h"
#include "GameFramework/RoomEscapeFPSPlayerState.h"
#include "Helper/Helper.h"
#include "Net/UnrealNetwork.h"

ATriggerObject::ATriggerObject()
{
	bIsTriggered = false;
}
void ATriggerObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}
void ATriggerObject::BeginPlay()
{
	Super::BeginPlay();
	if (OnTriggerEvent.IsBound() == false)
	{
		OnTriggerEvent.BindUFunction(this, *DelegateName);
	}
}

bool ATriggerObject::OnInteraction(APawn* requester, class UPrimitiveComponent* InComp)
{
	// 이미 동작한 트리거는 다시 동작하지 않음.
	if (bIsTriggered)
		return false;

	if (bNeedsUINotify)
	{
		if (Super::OnInteraction(requester, InComp) == false)
			return false;
	}
	else
	{
		if (AInteractiveObject::OnInteraction(requester, InComp) == false)
			return false;
	}
	
	
	bIsTriggered = true;
	OnTriggerEvent.ExecuteIfBound(requester);
	return true;
}

void ATriggerObject::ServerActivateGhostSpawner_Implementation(APawn* requester)
{
	if (GetNetMode() == NM_DedicatedServer)
	{
		ARoomEscapeFPSGameMode* gm = Helper::GetGameMode(GetWorld());
		if (gm)
		{
			gm->SetActiveGhostSpawner(true);
		}
	}
}
void ATriggerObject::ServerActivatePipeGame_Implementation(APawn* requester)
{
	ARoomEscapeFPSPlayerState* ps = requester->GetPlayerStateChecked<ARoomEscapeFPSPlayerState>();
	int32 id = ps->GetPlayerId();
	auto OpenPipeGame = [this, &ps]()
	{
		ps->InitializePipeGame(5);
	};
	Helper::ServerImplementToClient(GetWorld(), id, OpenPipeGame);
}

void ATriggerObject::ServerCompleteGameAndGotoMainMenu_Implementation(APawn* requester)
{
	UGameplayStatics::OpenLevel(requester, TEXT("Entry"));
}
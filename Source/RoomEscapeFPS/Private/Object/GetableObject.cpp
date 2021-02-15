// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/GetableObject.h"
#include "GameFramework/RoomEscapeFPSPlayerState.h"
#include "GameFramework/Pawn.h"
#include "Gameplay/TypeInfoHeader.h"
#include "Helper/Helper.h"

AGetableObject::AGetableObject()
{
	
}

void AGetableObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AGetableObject::BeginPlay()
{
	Super::BeginPlay();
	if (GetNetMode() == NM_DedicatedServer)
	{
		IsUseTimeline = false;
		CurrentState = EInteractiveObjectState::EState_Open_Or_On;
	}

	InformationStr = TEXT("Press 'E' key to get");
}

void AGetableObject::ToggleState(APawn* requester)
{
	if (GetNetMode() == NM_DedicatedServer && CurrentState == EInteractiveObjectState::EState_Open_Or_On)
	{
		check(requester);
		CurrentState = EInteractiveObjectState::EState_Close_Or_Off;
		Helper::SetActorActive(this, false);
		
		ARoomEscapeFPSPlayerState* ps = requester->GetPlayerStateChecked<ARoomEscapeFPSPlayerState>();
		int32 id = ps->GetPlayerId();
		auto AddItemToPlayerInventory = [this, &ps]()
		{
			// UI�� ����� �ϴ� Ư�� ������Ÿ�Կ� ���� ó��.
			if (ps->IsFirstGet(ItemType) && bFirstGetNeedsUpdateUI)
			{
				ps->ClientProcessHUDOnFirstItemGet(ItemType);
			}
			if (AdditionalItemType != EItemType::NONE)
			{	// �ķ��� ȹ���� ���, �ķ��� ȹ��� �Բ� ���͸��� �Ϻ� ��������� �Ѵ�.
				ps->AddItemToInventory(AdditionalItemType, AdditionalGetCount);
			}
			ps->AddItemToInventory(ItemType, DefaultGetCount);
		};
		Helper::ServerImplementToClient(GetWorld(), id, AddItemToPlayerInventory);
		Destroy();
	}
}

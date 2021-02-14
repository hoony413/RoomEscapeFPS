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
}

void AGetableObject::ToggleState(APawn* requester)
{
	if (GetNetMode() == NM_DedicatedServer && CurrentState == EInteractiveObjectState::EState_Open_Or_On)
	{
		check(requester);
		CurrentState = EInteractiveObjectState::EState_Close_Or_Off;
		Helper::SetActorActive(this, false);
		ARoomEscapeFPSPlayerState* ps = requester->GetPlayerStateChecked<ARoomEscapeFPSPlayerState>();
		
		if (ItemType == EItemType::EItemType_Battery)
		{
			ps->UpdateBatteryRemainValue(30.f);
		}
		else if (ItemType == EItemType::EItemType_Flash)
		{
			ps->UpdateBatteryRemainValue(90.f);
		}
		else
		{
			int32 id = ps->GetPlayerId();
			auto AddItemToPlayerInventory = [&]()
			{
				ps->AddItemToInventory(ItemType, DefaultGetCount);
			};
			Helper::ServerImplementToClient(GetWorld(), id, AddItemToPlayerInventory);
		}
		
		Destroy();
	}
}

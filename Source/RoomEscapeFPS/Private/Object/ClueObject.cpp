// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/ClueObject.h"
#include "Helper/Helper.h"
#include "GameFramework/RoomEscapeFPSPlayerState.h"

AClueObject::AClueObject()
{
	bNeedsUINotify = true;
}
void AClueObject::BeginPlay()
{
	Super::BeginPlay();
	InformationStr = TEXT("Press 'E' key to Check Clue");
}

void AClueObject::OnInteraction(class APawn* requester, class UPrimitiveComponent* InComp)
{
	if (GetNetMode() == NM_DedicatedServer)
	{
		check(requester);
		ARoomEscapeFPSPlayerState* ps = requester->GetPlayerStateChecked<ARoomEscapeFPSPlayerState>();
		int32 id = ps->GetPlayerId();
		auto ShowClue = [this, &ps]()
		{
			// 단서 정보 표시
			if (ps)
			{
				ps->ClientProcessHUDOnFirstItemGet(this);
			}
		};
		Helper::ServerImplementToClient(GetWorld(), id, ShowClue);
	}
}

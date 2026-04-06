// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/ClueObject.h"
#include "Helper/Helper.h"
#include "GameFramework/RoomEscapeFPSPlayerState.h"
#include "GameFramework/RoomEscapeFPSPlayerController.h"

AClueObject::AClueObject()
{
	bNeedsUINotify = true;
}
void AClueObject::BeginPlay()
{
	Super::BeginPlay();
}

bool AClueObject::OnInteraction(APawn* requester, UPrimitiveComponent* InComp)
{
	if (IsNetMode(NM_DedicatedServer))
	{
		check(requester);
		ARoomEscapeFPSPlayerState* ps = requester->GetPlayerStateChecked<ARoomEscapeFPSPlayerState>();
		int32 id = ps->GetPlayerId();
		auto const ShowClue = [this](ARoomEscapeFPSPlayerController* pc)
		{
			// 단서 정보 표시
			pc->ClientProcessHUDOnFirstItemGet(this);
		};
		Helper::ServerImplementToClient(GetWorld(), id, ShowClue);
	}

	return true;
}

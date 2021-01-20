// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Managers/UIManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/RoomEscapeFPSPlayerController.h"
#include "Managers/RoomEscapeFPSGameInstance.h"
/**
 * 유틸리티 함수 모음.
 */

#define DEBUG_TEXT_RED(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Red,text)
#define DEBUG_TEXT_GREEN(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)
#define DEBUG_BOX_BLUE(box, pos) DrawDebugBox(GetWorld(), pos, box->GetScaledBoxExtent(), FColor::Blue, true, -1, 0, 5);

#define GetUIMgr() Helper::GetManager<UUIManager>(GetWorld())


namespace Helper
{
	template<typename T>
	ROOMESCAPEFPS_API T* GetManager(UWorld* world)
	{
		URoomEscapeFPSGameInstance* gi = Cast<URoomEscapeFPSGameInstance>(world->GetGameInstance());
		return gi != nullptr ? gi->GetManager<T>() : nullptr;
	}

	ARoomEscapeFPSPlayerController* GetPlayerController(APawn* pawn);
	
}
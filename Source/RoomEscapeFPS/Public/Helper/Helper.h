// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Engine/AssetManager.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "GameFramework/RoomEscapeFPSGameMode.h"
#include "GameFramework/RoomEscapeFPSPlayerState.h"
#include "GameFramework/RoomEscapeFPSGameState.h"
#include "GameFramework/RoomEscapeFPSPlayerController.h"
//#include "Paper2D/Classes/PaperSprite.h"

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
	ROOMESCAPEFPS_API T* GetSubsystem(UWorld* world)
	{
		UGameInstance* gi = world->GetGameInstance();
		return IsValid(gi) ? gi->GetSubsystem<T>() : nullptr;
	}

	// world에서 pred 조건에 맞는 actor를 찾아준다.
	// 1. pred는 아래와 같은 형태의 lambda 함수를 사용하고
	//		auto functor = [&](T* e)->bool { return true/false; };
	// 2. 호출은 아래와 같이
	//		T* e = Helper::FindActor<T>(GetWorld(), functor);
	template <typename T, typename Functor>
	T* FindActor(UWorld* world, Functor functor)
	{
		ensure(world);
		for (TActorIterator<T> it(world); it; ++it)
		{
			if (functor(*it))
			{
				return *it;
			}
		}

		return nullptr;
	}

	template<typename Functor, typename... Args>
	ROOMESCAPEFPS_API void ServerImplementToClient(UWorld* world, int32 InPlayerID, Functor func, Args&&... args)
	{
		check(world->GetNetMode() == NM_DedicatedServer);
		ARoomEscapeFPSGameMode* gm = world->GetAuthGameMode<ARoomEscapeFPSGameMode>();
		if (not gm)
		{
			return;
		}

		ARoomEscapeFPSGameState* gs = gm->GetGameState<ARoomEscapeFPSGameState>();
		if (not gs)
		{
			return;
		}

		for (auto& elem : gs->PlayerArray)
		{
			ARoomEscapeFPSPlayerState* gsps = Cast<ARoomEscapeFPSPlayerState>(elem);
			if (gsps->GetPlayerId() == InPlayerID)
			{
				if (ARoomEscapeFPSPlayerController* pc = Cast<ARoomEscapeFPSPlayerController>(gsps->GetPlayerController()))
				{
					func(pc, Forward<Args>(args)...);
				}
			}
		}
	}

	ROOMESCAPEFPS_API void SetActorActive(class AActor* InActor, bool bActive);

	ROOMESCAPEFPS_API EServerSolutionType GetSolutionType(EServerSolutionResultType InType);
	ROOMESCAPEFPS_API EServerSolutionResultType GetSolutionResultType(EServerSolutionType InType);

	ROOMESCAPEFPS_API ARoomEscapeFPSGameMode* GetGameMode(UWorld* world);
	ROOMESCAPEFPS_API ARoomEscapeFPSGameState* GetGameState(UWorld* world);

	ROOMESCAPEFPS_API void UpdateNextUIInfo(UWorld* world, ENextInformationType curType, ENextInformationType nextType, int32 InCount);
}
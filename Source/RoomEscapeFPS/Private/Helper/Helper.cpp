// Fill out your copyright notice in the Description page of Project Settings.


#include "Helper/Helper.h"
#include "Engine/AssetManager.h"
#include "GameFrameWork/RoomEscapeFPSPlayerState.h"
#include "GameFramework/RoomEscapeFPSPlayerController.h"
#include "Gameplay/TypeInfoHeader.h"

namespace Helper
{
	ROOMESCAPEFPS_API void SetActorActive(class AActor* InActor, bool bActive)
	{
		ensure(InActor);

		// 액터를 Hidden 처리
		InActor->SetActorHiddenInGame(!bActive);
		
		// 액터의 충돌 검출 끄기
		InActor->SetActorEnableCollision(bActive);
		
		// 액터 틱 끄기
		InActor->SetActorTickEnabled(bActive);
		TArray<UActorComponent*> components;
		InActor->GetComponents(components);
		for (UActorComponent* c : components)
		{
			c->SetActive(bActive);
		}
	}

	ROOMESCAPEFPS_API EServerSolutionType GetSolutionType(EServerSolutionResultType InType)
	{
		switch (InType)
		{
		case EServerSolutionResultType::SOLUTION_RESULT_1:
			return EServerSolutionType::SOLUTION_1;
		case EServerSolutionResultType::SOLUTION_RESULT_2:
			return EServerSolutionType::SOLUTION_2;
		case EServerSolutionResultType::SOLUTION_RESULT_3:
			return EServerSolutionType::GHOST_DEAD_COUNT_TARGET;
		case EServerSolutionResultType::SOLUTION_RESULT_4:
			return EServerSolutionType::PIPELINE_GAME_COMPLETE;
		}
		return EServerSolutionType::NONE;
	}
	ROOMESCAPEFPS_API EServerSolutionResultType GetSolutionResultType(EServerSolutionType InType)
	{
		switch (InType)
		{
		case EServerSolutionType::SOLUTION_1:
			return EServerSolutionResultType::SOLUTION_RESULT_1;
		case EServerSolutionType::SOLUTION_2:
			return EServerSolutionResultType::SOLUTION_RESULT_2;
		case EServerSolutionType::GHOST_DEAD_COUNT_TARGET:
			return EServerSolutionResultType::SOLUTION_RESULT_3;
		case EServerSolutionType::PIPELINE_GAME_COMPLETE:
			return EServerSolutionResultType::SOLUTION_RESULT_4;
		}
		return EServerSolutionResultType::NONE;
	}

	ROOMESCAPEFPS_API ARoomEscapeFPSGameMode* GetGameMode(UWorld* world)
	{
		check(world->IsNetMode(NM_DedicatedServer));
		ARoomEscapeFPSGameMode* gm = world->GetAuthGameMode<ARoomEscapeFPSGameMode>();
		check(gm);
		return gm;
		
	}
	ROOMESCAPEFPS_API ARoomEscapeFPSGameState* GetGameState(UWorld* world)
	{
		ARoomEscapeFPSGameState* gs = world->GetGameState<ARoomEscapeFPSGameState>();
		check(gs);
		return gs;
	}

	ROOMESCAPEFPS_API void UpdateNextUIInfo(UWorld* world, ENextInformationType curType, ENextInformationType nextType, int32 InCount)
	{
		check(world);
		check(world->IsNetMode(NM_DedicatedServer));
		ARoomEscapeFPSGameMode* gm = world->GetAuthGameMode<ARoomEscapeFPSGameMode>();
		if (gm)
		{
			ARoomEscapeFPSGameState* gs = gm->GetGameState<ARoomEscapeFPSGameState>();
			if (gs)
			{
				for (auto& elem : gs->PlayerArray)
				{
					ARoomEscapeFPSPlayerState* ps = Cast<ARoomEscapeFPSPlayerState>(elem);
					if (ps)
					{
						ARoomEscapeFPSPlayerController* pc = Cast<ARoomEscapeFPSPlayerController>(ps->GetPlayerController());
						if (pc)
						{
							pc->ClientProcessHUDOnUpdateNextInfo(curType, nextType, InCount);
						}
					}
				}
			}
		}
	}
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "Helper/Helper.h"
#include "Runtime/Engine/Classes/Engine/AssetManager.h"
#include "GameFrameWork/RoomEscapeFPSPlayerState.h"
#include "Gameplay/TypeInfoHeader.h"
#include "Object/CharmProjectile.h"
//#include "Paper2D/Classes/PaperSprite.h"

namespace Helper
{
	ROOMESCAPEFPS_API TSharedPtr<FStreamableHandle> AsyncLoadResource(const FSoftObjectPath& assetRef, TFunction<void()>&& lambda)
	{
		FStreamableManager& assetLoader = UAssetManager::GetStreamableManager();
		return assetLoader.RequestAsyncLoad(TArray<FSoftObjectPath>{ assetRef }, FStreamableDelegate::CreateLambda(MoveTemp(lambda)));
	}

	ROOMESCAPEFPS_API AProjectileHandler* GetProjectileHandler(UWorld* world)
	{	// TODO: �ʿ� �ϳ��� �����ϹǷ� �ϴ� �̷��� ó��. ���� ����ó���� ���.
		for (TActorIterator<AProjectileHandler> it(world); it; ++it)
		{
			return *it;
		}

		return nullptr;
	}

	ROOMESCAPEFPS_API void SetActorActive(class AActor* InActor, bool bActive)
	{
		ensure(InActor);

		// ���͸� Hidden ó��
		InActor->SetActorHiddenInGame(!bActive);
		
		// ������ �浹 ���� ����
		InActor->SetActorEnableCollision(bActive);
		
		// ���� ƽ ����
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
		case EServerSolutionResultType::ESolutionResult_1:
			return EServerSolutionType::ESolution_1;
		case EServerSolutionResultType::ESolutionResult_2:
			return EServerSolutionType::ESolution_2;
		case EServerSolutionResultType::ESolutionResult_3:
			return EServerSolutionType::EGhostDeadCount_Target;
		case EServerSolutionResultType::ESolutionResult_4:
			return EServerSolutionType::EPipelineGame_Complete;
		}
		return EServerSolutionType::ENONE;
	}
	ROOMESCAPEFPS_API EServerSolutionResultType GetSolutionResultType(EServerSolutionType InType)
	{
		switch (InType)
		{
		case EServerSolutionType::ESolution_1:
			return EServerSolutionResultType::ESolutionResult_1;
		case EServerSolutionType::ESolution_2:
			return EServerSolutionResultType::ESolutionResult_2;
		case EServerSolutionType::EGhostDeadCount_Target:
			return EServerSolutionResultType::ESolutionResult_3;
		case EServerSolutionType::EPipelineGame_Complete:
			return EServerSolutionResultType::ESolutionResult_4;
		}
		return EServerSolutionResultType::ENONE;
	}

	ROOMESCAPEFPS_API ARoomEscapeFPSGameMode* GetGameMode(UWorld* world)
	{
		check(world->GetNetMode() == NM_DedicatedServer);
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
		check(world->GetNetMode() == NM_DedicatedServer);
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
						ps->ClientProcessHUDOnUpdateNextInfo(curType, nextType, InCount);
					}
				}
			}
		}
	}
}
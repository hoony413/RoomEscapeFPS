// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Managers/UIManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/RoomEscapeFPSPlayerController.h"
#include "Managers/RoomEscapeFPSGameInstance.h"
#include "Runtime/Engine/Classes/Engine/AssetManager.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "Gameplay/ProjectileHandler.h"
#include "GameFramework/RoomEscapeFPSGameMode.h"
#include "GameFramework/RoomEscapeFPSPlayerState.h"
#include "GameFramework/RoomEscapeFPSGameState.h"
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
	ROOMESCAPEFPS_API T* GetManager(UWorld* world)
	{
		URoomEscapeFPSGameInstance* gi = Cast<URoomEscapeFPSGameInstance>(world->GetGameInstance());
		return gi != nullptr ? gi->GetManager<T>() : nullptr;
	}

	template <typename T>
	ROOMESCAPEFPS_API FORCEINLINE T* SyncLoadResource(const FString& path)
	{
		check(!path.IsEmpty());
		return LoadObject<T>(NULL, *path);
	}
	
	ROOMESCAPEFPS_API TSharedPtr<FStreamableHandle> AsyncLoadResource(const FSoftObjectPath& assetRef, TFunction<void()>&& lambda);

	//ROOMESCAPEFPS_API FORCEINLINE void SetSprite(UPaperSprite* InSprite, FSlateBrush& InBrush)
	//{
	//	check(InSprite);
	//	InBrush.SetResourceObject(InSprite);
	//}

	// world에서 pred 조건에 맞는 actor를 찾아준다.
	// 1. pred는 아래와 같은 형식의 lambda 함수를 사용하고
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

	template<typename Functor>
	ROOMESCAPEFPS_API void ServerImplementToClient(UWorld* world, int32 InPlayerID, Functor func)
	{
		//check(GetNetMode() == NM_DedicatedServer);
		ARoomEscapeFPSGameMode* gm = world->GetAuthGameMode<ARoomEscapeFPSGameMode>();
		if (gm)
		{
			ARoomEscapeFPSGameState* gs = gm->GetGameState<ARoomEscapeFPSGameState>();
			if (gs)
			{
				for (auto& elem : gs->PlayerArray)
				{
					ARoomEscapeFPSPlayerState* gsps = Cast<ARoomEscapeFPSPlayerState>(elem);
					if (gsps->GetPlayerId() == InPlayerID)
					{
						func();
					}
				}
			}
		}
	}

	ROOMESCAPEFPS_API AProjectileHandler* GetProjectileHandler(UWorld* world);
	ROOMESCAPEFPS_API void SetActorActive(class AActor* InActor, bool bActive);

	ROOMESCAPEFPS_API EServerSolutionType GetSolutionType(EServerSolutionResultType InType);
	ROOMESCAPEFPS_API EServerSolutionResultType GetSolutionResultType(EServerSolutionType InType);

	// Convert Euler Rotations To Quaternions
	ROOMESCAPEFPS_API FQuat Euler_To_Quaternion(FRotator& Current_Rotation);
	// Function to set world rotation of scene component to input quaternion rotation
	ROOMESCAPEFPS_API void SetWorldRotationQuat(USceneComponent* SceneComponent, const FQuat& Desired_Rotation);
	// Function to set relative rotation of scene component to input quaternion rotation
	ROOMESCAPEFPS_API void SetRelativeRotationQuat(USceneComponent* SceneComponent, const FQuat& Desired_Rotation);
	// Function to add delta rotation to current local rotation of scene component
	ROOMESCAPEFPS_API void AddLocalRotationQuat(USceneComponent* SceneComponent, const FQuat& Delta_Rotation);
	// Function to set world rotation of Actor to input quaternion rotation
	ROOMESCAPEFPS_API void SetActorWorldRotationQuat(AActor* Actor, const FQuat& Desired_Rotation);
	// Function to set relative rotation of Actor to input quaternion rotation
	ROOMESCAPEFPS_API void SetActorRelativeRotationQuat(AActor* Actor, const FQuat& Desired_Rotation);
	// Function to add delta rotation to current local rotation of Actor
	ROOMESCAPEFPS_API void AddActorLocalRotationQuat(AActor* Actor, const FQuat& Delta_Rotation);
}
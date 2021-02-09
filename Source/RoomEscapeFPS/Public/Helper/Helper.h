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

	ROOMESCAPEFPS_API AProjectileHandler* GetProjectileHandler(UWorld* world);
}
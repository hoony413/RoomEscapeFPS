// Fill out your copyright notice in the Description page of Project Settings.


#include "Helper/Helper.h"
#include "Runtime/Engine/Classes/Engine/AssetManager.h"
//#include "Paper2D/Classes/PaperSprite.h"

namespace Helper
{
	ROOMESCAPEFPS_API TSharedPtr<FStreamableHandle> AsyncLoadResource(const FSoftObjectPath& assetRef, TFunction<void()>&& lambda)
	{
		FStreamableManager& assetLoader = UAssetManager::GetStreamableManager();
		return assetLoader.RequestAsyncLoad(TArray<FSoftObjectPath>{ assetRef }, FStreamableDelegate::CreateLambda(MoveTemp(lambda)));
	}

	ROOMESCAPEFPS_API AProjectileHandler* GetProjectileHandler(UWorld* world)
	{
		int32 i = 0;
		for (TActorIterator<AProjectileHandler> it(world); it; ++it)
		{
			return *it;
		}
		return nullptr;
	}
}
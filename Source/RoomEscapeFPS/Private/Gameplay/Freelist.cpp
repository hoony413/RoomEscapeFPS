// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Freelist.h"


UFreelist::UFreelist()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryComponentTick.bCanEverTick = false;

}

// Called when the game starts
void UFreelist::BeginPlay()
{
	Super::BeginPlay();

	FreeList.Reserve(100);
}
void UFreelist::BeginDestroy()
{
	ReleaseFreeList();
	Super::BeginDestroy();
}
void UFreelist::ReleaseFreeList()
{
	for (auto& elem : FreeList)
	{
		if (elem != nullptr && !elem->IsPendingKillOrUnreachable())
		{
			elem->Destroy();
		}
	}
	FreeList.Empty();
}
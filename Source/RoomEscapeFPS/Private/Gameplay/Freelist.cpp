// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Freelist.h"


UFreelist::UFreelist()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryComponentTick.bCanEverTick = true;

}

// Called when the game starts
void UFreelist::BeginPlay()
{
	Super::BeginPlay();

	FreeList.Reserve(100);
}

// Called every frame
void UFreelist::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UFreelist::ReleaseFreeList()
{
	for (auto& elem : FreeList)
	{
		elem->Destroy();
	}
	FreeList.Empty();
}
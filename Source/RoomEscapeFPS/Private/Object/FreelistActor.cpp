// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/FreelistActor.h"

// Sets default values
AFreelistActor::AFreelistActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFreelistActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFreelistActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFreelistActor::SetIsInFreeList(bool bFreeList)
{
	// FreeList에 넣는다면
	bIsInFreeList = bFreeList;
	// 액터를 Hidden 처리
	SetActorHiddenInGame(bIsInFreeList);
	// 액터의 충돌 검출 끄기
	SetActorEnableCollision(!bIsInFreeList);
	// 액터 틱 끄기
	SetActorTickEnabled(!bIsInFreeList);
}
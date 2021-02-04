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
	// FreeList�� �ִ´ٸ�
	bIsInFreeList = bFreeList;
	// ���͸� Hidden ó��
	SetActorHiddenInGame(bIsInFreeList);
	// ������ �浹 ���� ����
	SetActorEnableCollision(!bIsInFreeList);
	// ���� ƽ ����
	SetActorTickEnabled(!bIsInFreeList);
}
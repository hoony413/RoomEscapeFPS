// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/GhostSoul.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/GhostAIController.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Net/UnrealNetwork.h"

AGhostSoul::AGhostSoul()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	SphereCol = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCol"));
	SetRootComponent(SphereCol);

	MoveToLocationBoundingBox = CreateDefaultSubobject<UBoxComponent>(TEXT("MovableBoundingBox"));
	MoveToLocationBoundingBox->SetupAttachment(RootComponent);

	// Create a gun mesh component
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	BodyMesh->bCastDynamicShadow = false;
	BodyMesh->CastShadow = false;
	BodyMesh->SetupAttachment(RootComponent);

	GhostMovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("GhostMovement"));
	if (GhostMovementComponent)
	{
		GhostMovementComponent->UpdatedComponent = SphereCol;
	}

	AIControllerClass = AGhostAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void AGhostSoul::BeginPlay()
{
	Super::BeginPlay();

	bIsInFreeList = false;
	SphereCol->SetSphereRadius(100.f);
	AGhostAIController* ghostAI = Cast<AGhostAIController>(AIControllerClass);
	if (ghostAI)
	{
		ghostAI->SetGhostState(EGhostStateMachine::EIdle);
	}
}
UBoxComponent* AGhostSoul::GetBoundingBox()
{
	check(MoveToLocationBoundingBox);
	return MoveToLocationBoundingBox;
}
void AGhostSoul::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//DOREPLIFETIME(AGhostSoul, SphereRadius);
	//DOREPLIFETIME(AGhostSoul, BonudingBoxSize);
}

bool AGhostSoul::IsInFreeList()
{
	return bIsInFreeList;
}
void AGhostSoul::SetIsInFreeList(bool bFreeList)
{
	bIsInFreeList = bFreeList;
	// 액터를 Hidden 처리
	SetActorHiddenInGame(bIsInFreeList);
	// 액터의 충돌 검출 끄기
	SetActorEnableCollision(!bIsInFreeList);
	// 액터 틱 끄기
	SetActorTickEnabled(!bIsInFreeList);
}

#if WITH_EDITOR
void AGhostSoul::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (PropertyChangedEvent.MemberProperty)
	{
		FName propertyName = PropertyChangedEvent.MemberProperty->GetFName();
		if (propertyName == GET_MEMBER_NAME_CHECKED(AGhostSoul, SphereRadius))
		{
			SphereCol->SetSphereRadius(SphereRadius);
		}
		else if (propertyName == GET_MEMBER_NAME_CHECKED(AGhostSoul, BonudingBoxSize))
		{
			MoveToLocationBoundingBox->SetBoxExtent(BonudingBoxSize);
		}
	}
}
#endif
// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/GhostSoul.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/GhostAIController.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Helper/Helper.h"
#include "Gameplay/GhostSpawner.h"
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

	// Create a mesh component
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	BodyMesh->bCastDynamicShadow = false;
	BodyMesh->CastShadow = false;
	BodyMesh->SetupAttachment(RootComponent);

	GhostMovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("GhostMovement"));
	if (GhostMovementComponent)
	{
		GhostMovementComponent->SetUpdatedComponent(SphereCol);
	}

	AIControllerClass = AGhostAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void AGhostSoul::BeginPlay()
{
	Super::BeginPlay();

	SetIsInFreeList(false);
	bMarkDead = false;
	SphereCol->SetSphereRadius(100.f);
}
UBoxComponent* AGhostSoul::GetBoundingBox()
{
	check(MoveToLocationBoundingBox);
	return MoveToLocationBoundingBox;
}

void AGhostSoul::PlayDeadAnimation()
{
	fDeadAnimStartTime = GetWorld()->GetTimeSeconds();
	bMarkDead = true;
	mid = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, DeadMaterialInstance);
	if (mid)
	{
		defaultMat = Cast<UMaterialInstance>(BodyMesh->GetMaterial(0));
		BodyMesh->SetMaterial(0, mid);
	}
}

void AGhostSoul::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bMarkDead && mid)
	{
		float CurrentValue = 1 - (GetWorld()->GetTimeSeconds() - fDeadAnimStartTime);
		if (CurrentValue >= 0.f)
		{
			mid->SetVectorParameterValue(TEXT("BaseColor"), FLinearColor(CurrentValue, CurrentValue, CurrentValue));
			mid->SetScalarParameterValue(TEXT("TransparencyAmount"), CurrentValue);
		}
		else
		{
			bMarkDead = false;
			if (GetNetMode() == NM_DedicatedServer)
			{
				ServerReturnGhostElement();
			}
		}
	}
}
void AGhostSoul::ServerReturnGhostElement_Implementation()
{
	if (GetNetMode() == NM_DedicatedServer)
	{
		int32 count = 0;
		auto functor = [&](AGhostSpawner* e)->bool 
		{
			++count;
			return true;
		};

		AGhostSpawner* spawner = Helper::FindActor<AGhostSpawner>(GetWorld(), functor);
		if (spawner)
		{
			spawner->DeactiveGhost(this);
		}

		check(count == 1);
	}
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
	Helper::SetActorActive(this, !bFreeList);

	// 오버랩 이벤트 끄기
	SphereCol->SetGenerateOverlapEvents(!bFreeList);

	// 리플리케이트 옵션 끄기
	bReplicates = !bFreeList;

	//if (bIsInFreeList)
	//{
	//	BodyMesh->SetMaterial(0, defaultMat);
	//}
	if(!bFreeList)
	{
		AGhostAIController* controller = Cast<AGhostAIController>(GetController());
		if (controller)
		{
			controller->SetGhostState(EGhostStateMachine::EIdle);
		}
	}
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
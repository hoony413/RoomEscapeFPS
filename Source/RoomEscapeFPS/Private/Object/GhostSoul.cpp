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
#include "Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Net/UnrealNetwork.h"

AGhostSoul::AGhostSoul()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

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

	GhostParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("GhostParticle"));
	GhostParticle->SetupAttachment(RootComponent);
	GhostParticle->bAutoActivate = true;
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("ParticleSystem'/Game/Resources/Materials/GhostFlaming_Particle.GhostFlaming_Particle'"));
	if (ParticleAsset.Succeeded())
	{
		GhostParticle->SetTemplate(ParticleAsset.Object);
	}

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
	SphereCol->SetSphereRadius(100.f);
}
UBoxComponent* AGhostSoul::GetBoundingBox()
{
	check(MoveToLocationBoundingBox);
	return MoveToLocationBoundingBox;
}

void AGhostSoul::SetAsDead()
{
	if (GetNetMode() == NM_DedicatedServer)
	{
		auto GhostSpawnerFinder = [&](AGhostSpawner* e)->bool
		{
			return true;
		};

		AGhostSpawner* spawner = Helper::FindActor<AGhostSpawner>(GetWorld(), GhostSpawnerFinder);
		if (spawner)
		{
			spawner->DeactiveGhost(this);
		}
	}
}

void AGhostSoul::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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

	if(!bFreeList)
	{
		AGhostAIController* controller = Cast<AGhostAIController>(GetController());
		if (controller)
		{
			controller->SetGhostState(EGhostStateMachine::EIdle);
		}
		// TODO: 풀에 있는 고스트 사용 시 파티클 잔상이 기존 위치에 남아서 보이는 문제가 있어 
		// 고스트 리스폰 시 딜레이를 줘야 한다(아래 코드가 왜 의도대로 되지 않는지 확인 필요)
		//GhostParticle->SetVisibility(false);
		//GetWorld()->GetTimerManager().SetTimer(EmitterDelayTimer, this, &AGhostSoul::DelayActivateParticle,
		//	1.0f, false);
	}
}

void AGhostSoul::DelayActivateParticle()
{
	//GhostParticle->SetVisibility(true);
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
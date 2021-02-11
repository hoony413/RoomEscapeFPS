// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/CharmProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Helper/Helper.h"
#include "Gameplay/ProjectileHandler.h"
#include "Object/GhostSoul.h"
#include "GameFramework/GhostAIController.h"

// Sets default values
ACharmProjectile::ACharmProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SphereCol = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCol"));
	//SphereCol->SetupAttachment(RootComponent);
	SetRootComponent(SphereCol);

	CharmMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CharmPlaneMesh"));
	//SetRootComponent(CharmMesh);
	CharmMesh->SetupAttachment(RootComponent);

	ProjMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("CharmMovement"));
	if (ProjMovement)
	{
		ProjMovement->SetUpdatedComponent(SphereCol);
		ProjMovement->InitialSpeed = 2400.0f;
		ProjMovement->MaxSpeed = 2400.0f;
		ProjMovement->bRotationFollowsVelocity = true;
	}
}

// Called when the game starts or when spawned
void ACharmProjectile::BeginPlay()
{
	Super::BeginPlay();

	AActor* owner = GetOwner();
	SphereCol->IgnoreActorWhenMoving(owner, true);
	if (SphereCol->OnComponentBeginOverlap.IsBound() == false)
	{
		SphereCol->OnComponentBeginOverlap.AddDynamic(this, &ACharmProjectile::OnComponentBeginOverlap);
	}

	Instigator = owner;
}

void ACharmProjectile::Fire(const FVector& pos, const FVector& dir)
{
	if (GetNetMode() == NM_DedicatedServer)
	{
		NetMulticastFire(pos, dir);
	}
}
void ACharmProjectile::NetMulticastFire_Implementation(const FVector& pos, const FVector& dir)
{
	check(ProjMovement);
	SetActorLocation(pos);
	SetActorRotation(dir.Rotation());
	ProjMovement->Velocity = (dir * ProjMovement->InitialSpeed);
}

void ACharmProjectile::OnComponentBeginOverlap
(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == nullptr)
		return;

	if (Instigator.IsValid() && Instigator.Get() == OtherActor)
		return;

	AProjectileHandler* handler = Helper::GetProjectileHandler(GetWorld());
	if (handler)
	{
		handler->ReturnCharm(this);
	}

	if (OtherActor->IsA<AGhostSoul>())
	{
		AGhostSoul* ghost = Cast<AGhostSoul>(OtherActor);
		if (ghost)
		{
			AGhostAIController* ghostAI = Cast<AGhostAIController>(ghost->GetController());
			if (ghostAI)
			{
				if (ghostAI->GetGhostState() != EGhostStateMachine::EDead)
				{	// SetGhostState 안에서 사망 처리 Multicast가 서버로부터 호출된다.
					ghostAI->SetGhostState(EGhostStateMachine::EDead);
				}
			}
		}
	}
}
void ACharmProjectile::OnProjectileStop(const FHitResult& hitResult)
{
	// 서버: 탄체 삭제 / 고스트 피격 판정 시 고스트 사망 멀티캐스트
	if (GetNetMode() == NM_DedicatedServer)
	{
		AProjectileHandler* handler = Helper::GetProjectileHandler(GetWorld());
		if (handler)
		{
			handler->ReturnCharm(this);
		}

		if (hitResult.Actor.IsValid())
		{
			if (hitResult.Actor.Get()->IsA<AGhostSoul>())
			{
				AGhostSoul* ghost = Cast<AGhostSoul>(hitResult.Actor.Get());
				if (ghost)
				{
					AGhostAIController* ghostAI = Cast<AGhostAIController>(ghost->GetController());
					if (ghostAI)
					{
						if (ghostAI->GetGhostState() != EGhostStateMachine::EDead)
						{	// SetGhostState 안에서 사망 처리 후 Multicast가 서버로부터 호출된다.
							ghostAI->SetGhostState(EGhostStateMachine::EDead);
						}
					}
				}
			}
		}
	}
}

// Called every frame
void ACharmProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

bool ACharmProjectile::IsInFreeList()
{
	return bIsInFreeList;
}
void ACharmProjectile::SetIsInFreeList(bool bFreeList)
{
	bIsInFreeList = bFreeList;
	Helper::SetActorActive(this, !bFreeList);

	// 오버랩 이벤트 끄기
	SphereCol->SetGenerateOverlapEvents(!bFreeList);

	// 리플리케이트 옵션 끄기
	bReplicates = !bFreeList;
}

#if WITH_EDITOR
void ACharmProjectile::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (PropertyChangedEvent.MemberProperty)
	{
		FName propertyName = PropertyChangedEvent.MemberProperty->GetFName();
		if (propertyName == GET_MEMBER_NAME_CHECKED(ACharmProjectile, SphereRadius))
		{
			SphereCol->SetSphereRadius(SphereRadius);
		}
	}
}
#endif
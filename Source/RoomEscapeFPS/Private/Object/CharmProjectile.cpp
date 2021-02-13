// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/CharmProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Helper/Helper.h"
#include "Gameplay/ProjectileHandler.h"
#include "Object/GhostSoul.h"
#include "GameFramework/GhostAIController.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
//#include "Object/ProjectileExplodeEffect.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ACharmProjectile::ACharmProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SphereCol = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCol"));
	SetRootComponent(SphereCol);

	SphereCol->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	SphereCol->CanCharacterStepUpOn = ECB_No;

	CharmMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CharmPlaneMesh"));
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

void ACharmProjectile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACharmProjectile, fLifeTime);
	DOREPLIFETIME(ACharmProjectile, fLifeStartTime);
}

// Called when the game starts or when spawned
void ACharmProjectile::BeginPlay()
{
	Super::BeginPlay();

	AActor* owner = GetOwner();
	SphereCol->IgnoreActorWhenMoving(owner, true);
	if (SphereCol->OnComponentHit.IsBound() == false)
	{
		SphereCol->OnComponentHit.AddDynamic(this, &ACharmProjectile::OnComponentHit);
	}

	Instigator = owner;

	if (GetNetMode() == NM_DedicatedServer)
	{
		fLifeTime = 0.5f;
		fLifeStartTime = 0.f;
	}
}

void ACharmProjectile::Fire(const FVector& pos, const FVector& dir)
{
	if (GetNetMode() == NM_DedicatedServer)
	{
		NetMulticastFire(pos, dir);
		fLifeStartTime = GetWorld()->GetTimeSeconds();
	}
}
void ACharmProjectile::NetMulticastFire_Implementation(const FVector& pos, const FVector& dir)
{
	check(ProjMovement);
	SetActorLocation(pos);
	SetActorRotation(dir.Rotation());
	ProjMovement->Velocity = (dir * ProjMovement->InitialSpeed);
}

void ACharmProjectile::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor == nullptr)
		return;
	if (Instigator.IsValid() && Instigator.Get() == OtherActor)
		return;

	if (OtherActor->IsA<AGhostSoul>())
	{
		AGhostSoul* ghost = Cast<AGhostSoul>(OtherActor);
		if (ghost)
		{
			AGhostAIController* ghostAI = Cast<AGhostAIController>(ghost->GetController());
			if (ghostAI)
			{
				if (ghostAI->GetGhostState() != EGhostStateMachine::EDead)
				{	
					// 고스트를 맞춘 경우만 폭발 이펙트를 RPC한다.
					NetMulticastProjectileExplode();

					// SetGhostState 안에서 사망 처리 Multicast가 서버로부터 호출된다.
					ghostAI->SetGhostState(EGhostStateMachine::EDead);
				}
			}
		}
	}
	else
	{
		if (GetNetMode() == NM_DedicatedServer)
		{
			DeactiveCharm();
		}
	}
}

void ACharmProjectile::NetMulticastProjectileExplode_Implementation()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), 
		ExplosionParticle.LoadSynchronous(), GetActorLocation(), FRotator::ZeroRotator, true, EPSCPoolMethod::AutoRelease);
	if (GetNetMode() == NM_DedicatedServer)
	{
		DeactiveCharm();
	}
}
void ACharmProjectile::DeactiveCharm()
{
	AProjectileHandler* handler = Helper::GetProjectileHandler(GetWorld());
	if (handler)
	{
		handler->ReturnCharm(this);
	}
}
// Called every frame
void ACharmProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetNetMode() == NM_DedicatedServer)
	{
		if (fLifeStartTime + fLifeTime < GetWorld()->GetTimeSeconds())
		{
			DeactiveCharm();
		}
	}
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
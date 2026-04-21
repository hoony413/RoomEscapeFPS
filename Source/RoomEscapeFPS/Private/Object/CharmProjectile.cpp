// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/CharmProjectile.h"

#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Helper/Helper.h"
#include "Object/GhostSoul.h"
#include "GameFramework/GhostAIController.h"
#include "UObject/ConstructorHelpers.h"
#include "Particles/ParticleSystem.h"
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

	if (IsNetMode(NM_DedicatedServer))
	{
		fLifeTime = 0.5f;
		fLifeStartTime = 0.f;
	}
}

void ACharmProjectile::Fire(const FVector& pos, const FVector& dir)
{
	if (IsNetMode(NM_DedicatedServer))
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
	if (not IsValid(OtherActor))
	{
		return;
	}
	if (Instigator.IsValid() && Instigator.Get() == OtherActor)
	{
		return;
	}

	if (OtherActor->IsA<AGhostSoul>())
	{
		AGhostSoul* ghost = Cast<AGhostSoul>(OtherActor);
		if (ghost)
		{
			AGhostAIController* ghostAI = Cast<AGhostAIController>(ghost->GetController());
			if (ghostAI)
			{
				if (ghostAI->GetGhostState() != EGhostStateMachine::DEAD)
				{
					// 고스트를 맞춘 경우만 폭발 이펙트를 RPC한다.
					NetMulticastProjectileExplode();

					// SetGhostState 안에서 사망 처리 Multicast가 서버로부터 호출된다.
					ghostAI->SetGhostState(EGhostStateMachine::DEAD);
				}
			}
		}
	}
	else
	{
		if (IsNetMode(NM_DedicatedServer))
		{
			Destroy();
		}
	}
}

void ACharmProjectile::NetMulticastProjectileExplode_Implementation()
{
	if (IsNetMode(NM_Client))
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), 
			ExplosionObj.LoadSynchronous(), GetActorLocation(), FRotator::ZeroRotator, FVector::OneVector, true, true, ENCPoolMethod::AutoRelease);
	}
	else if (IsNetMode(NM_DedicatedServer))
	{
		Destroy();
	}
}

// Called every frame
void ACharmProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsNetMode(NM_DedicatedServer))
	{
		if (fLifeStartTime + fLifeTime < GetWorld()->GetTimeSeconds())
		{
			Destroy();
		}
	}
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
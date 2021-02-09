// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/CharmProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/BillboardComponent.h"

// Sets default values
ACharmProjectile::ACharmProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("CharmBillboard"));
	SetRootComponent(Billboard);

	SphereCol = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCol"));
	SphereCol->SetupAttachment(RootComponent);

	ProjMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("CharmMovement"));
	if (ProjMovement)
	{
		ProjMovement->SetUpdatedComponent(Billboard);
		ProjMovement->InitialSpeed = 3000.0f;
		ProjMovement->MaxSpeed = 3000.0f;
		ProjMovement->bRotationFollowsVelocity = true;
		ProjMovement->bShouldBounce = true;
		ProjMovement->Bounciness = 0.3f;
	}
}

// Called when the game starts or when spawned
void ACharmProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (ProjMovement->OnProjectileStop.IsBound() == false)
	{
		ProjMovement->OnProjectileStop.AddDynamic(this, &ACharmProjectile::OnProjectileStop);
	}
}

void ACharmProjectile::SetInstigator(class AActor* InInstigator)
{
	Instigator = InInstigator;
	SphereCol->IgnoreActorWhenMoving(Instigator.Get(), true);
}

void ACharmProjectile::Fire(const FVector& pos, const FVector& dir)
{
	check(ProjMovement);
	SetActorLocation(pos);
	SetActorRotation(dir.Rotation());
	ProjMovement->Velocity = (GetActorRotation().Vector() * ProjMovement->InitialSpeed);
	//ProjMovement->SetVelocityInLocalSpace(dir * ProjMovement->InitialSpeed);
}

void ACharmProjectile::OnProjectileStop(const FHitResult& hitResult)
{
	// ��Ʈ ����(����)
	// ��Ʈ ���� ���� ��Ƽĳ��Ʈ?
	int32 a = 10;
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
	// ���͸� Hidden ó��
	SetActorHiddenInGame(bIsInFreeList);
	// ������ �浹 ���� ����
	SetActorEnableCollision(!bIsInFreeList);
	// ���� ƽ ����
	SetActorTickEnabled(!bIsInFreeList);

	//ProjMovement->OnProjectileStop
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
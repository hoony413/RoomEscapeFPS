// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/ProjectileHandler.h"
#include "Gameplay/Freelist.h"
#include "Object/CharmProjectile.h"

// Sets default values
AProjectileHandler::AProjectileHandler()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	CharmFreelist = CreateDefaultSubobject<UFreelist>(TEXT("CharmFreelist"));
}

// Called when the game starts or when spawned
void AProjectileHandler::BeginPlay()
{
	Super::BeginPlay();
	
}
class ACharmProjectile* AProjectileHandler::GetCharm(class APawn* InOwner)
{
	check(CharmFreelist);
	return CharmFreelist->GetElement<ACharmProjectile>(InOwner);
}
void AProjectileHandler::ReturnCharm(class ACharmProjectile* InCharm)
{
	check(CharmFreelist);
	CharmFreelist->ReturnElement<ACharmProjectile>(InCharm);
}

// Called every frame
void AProjectileHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


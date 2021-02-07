// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/GhostSoul.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"

AGhostSoul::AGhostSoul()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	SphereCol = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCol"));
	SetRootComponent(SphereCol);

	// Create a gun mesh component
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	BodyMesh->bCastDynamicShadow = false;
	BodyMesh->CastShadow = false;
	BodyMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AGhostSoul::BeginPlay()
{
	Super::BeginPlay();

	SphereCol->SetSphereRadius(100.f);
}

void AGhostSoul::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AGhostSoul, SphereRadius);
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
	}
}
#endif
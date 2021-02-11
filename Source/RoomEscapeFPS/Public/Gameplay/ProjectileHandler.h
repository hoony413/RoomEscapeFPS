// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileHandler.generated.h"

UCLASS()
class ROOMESCAPEFPS_API AProjectileHandler : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileHandler();

	class ACharmProjectile* GetCharm(class APawn* InOwner);
	void ReturnCharm(class ACharmProjectile* InCharm);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UFreelist* CharmFreelist;
};

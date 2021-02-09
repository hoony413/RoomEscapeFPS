// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Object/FreelistObjectInterface.h"
#include "CharmProjectile.generated.h"

UCLASS()
class ROOMESCAPEFPS_API ACharmProjectile : public AActor, public IFreelistObjectInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACharmProjectile();

	virtual bool IsInFreeList() override;
	virtual void SetIsInFreeList(bool bFreeList) override;

	void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

	void SetInstigator(class AActor* InInstigator);
	void Fire(const FVector& pos, const FVector& dir);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FOnProjectileStopDelegate, const FHitResult&, ImpactResult );
	UFUNCTION()
		void OnProjectileStop(const FHitResult& hitResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:
	UPROPERTY()
		bool bIsInFreeList;

private:

	UPROPERTY(VisibleDefaultsOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* SphereCol;

	UPROPERTY(VisibleDefaultsOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
		class UProjectileMovementComponent* ProjMovement;

	UPROPERTY(VisibleDefaultsOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
		class UBillboardComponent* Billboard;

	UPROPERTY(/*Replicated, */EditAnywhere, BlueprintReadOnly, Category = "Projectile Col Size", Meta = (AllowPrivateAccess = "true"))
		float SphereRadius;

	TWeakObjectPtr<class AActor> Instigator;
	TWeakObjectPtr<class AActor> Victim;
};

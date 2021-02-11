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

	void Fire(const FVector& pos, const FVector& dir);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//DELEGATE_OneParam( FOnProjectileStopDelegate, const FHitResult&, ImpactResult );
	UFUNCTION()
		void OnProjectileStop(const FHitResult& hitResult);

	//DELEGATE_SixParams(FComponentBeginOverlapSignature, UPrimitiveComponent, OnComponentBeginOverlap, UPrimitiveComponent*, OverlappedComponent, AActor*, OtherActor, UPrimitiveComponent*, OtherComp, int32, OtherBodyIndex, bool, bFromSweep, const FHitResult &, SweepResult)
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(NetMulticast, Reliable)
		void NetMulticastFire(const FVector& pos, const FVector& dir);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:
	UPROPERTY()
		bool bIsInFreeList;

private:

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* CharmMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* SphereCol;

	UPROPERTY(VisibleDefaultsOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		class UProjectileMovementComponent* ProjMovement;

	UPROPERTY(/*Replicated, */EditAnywhere, BlueprintReadOnly, Category = "Projectile Col Size", Meta = (AllowPrivateAccess = "true"))
		float SphereRadius;

	TWeakObjectPtr<class AActor> Instigator;
	TWeakObjectPtr<class AActor> Victim;
};

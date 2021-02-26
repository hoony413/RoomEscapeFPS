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

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void Fire(const FVector& pos, const FVector& dir);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_FiveParams(FComponentHitSignature, UPrimitiveComponent, OnComponentHit, UPrimitiveComponent*, HitComponent, AActor*, OtherActor, UPrimitiveComponent*, OtherComp, FVector, NormalImpulse, const FHitResult&, Hit);
	UFUNCTION()
		void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(NetMulticast, Reliable)
		void NetMulticastFire(const FVector& pos, const FVector& dir);

	UFUNCTION(NetMulticast, Unreliable)
		void NetMulticastProjectileExplode();


	void DeactiveCharm();

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

	UPROPERTY(EditAnywhere, Category = Particle, meta = (AllowPrivateAccess = "true"))
		TSoftObjectPtr<UParticleSystem> ExplosionParticle;

	UPROPERTY(VisibleDefaultsOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		class UProjectileMovementComponent* ProjMovement;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Projectile Col Size", Meta = (AllowPrivateAccess = "true"))
		float SphereRadius;

	TWeakObjectPtr<class AActor> Instigator;
	TWeakObjectPtr<class AActor> Victim;

	UPROPERTY()
		float fLifeTime = 0.5f;
	UPROPERTY()
		float fLifeStartTime = 0.f;
};

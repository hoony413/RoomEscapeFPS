// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CharmProjectile.generated.h"

class UNiagaraSystem;
class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class ROOMESCAPEFPS_API ACharmProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACharmProjectile();

#if WITH_EDITOR
	void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void Fire(const FVector& pos, const FVector& dir);

protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;

	//DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_FiveParams(FComponentHitSignature, UPrimitiveComponent, OnComponentHit, UPrimitiveComponent*, HitComponent, AActor*, OtherActor, UPrimitiveComponent*, OtherComp, FVector, NormalImpulse, const FHitResult&, Hit);
	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastFire(const FVector& pos, const FVector& dir);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastProjectileExplode();

public:	
	// Called every frame
	void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> CharmMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> SphereCol;

	UPROPERTY(EditAnywhere, Category = Niagara, meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<UNiagaraSystem> ExplosionObj;

	UPROPERTY(VisibleDefaultsOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UProjectileMovementComponent> ProjMovement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile Col Size", Meta = (AllowPrivateAccess = "true"))
	float SphereRadius;

	TWeakObjectPtr<class AActor> Instigator;
	TWeakObjectPtr<class AActor> Victim;

	UPROPERTY()
	float fLifeTime = 0.5f;

	UPROPERTY()
	float fLifeStartTime = 0.f;
};

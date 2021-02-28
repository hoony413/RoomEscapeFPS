// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/FreelistObjectInterface.h"
#include "GameFramework/Pawn.h"
#include "GhostSoul.generated.h"

/**
 * 생성: GhostSpawner->GhostFreelist 이후 GhostAIController에서 액터 제어
 * 소멸(Deactive): Ghost(피격 판정받음)->GhostAIController(사라짐 상태 변경)->
 *					Ghost(사라짐 연출 후 종료 델리게이트)->GhostSpawner(오브젝트 반납)->GhostFreelist
 */
UCLASS()
class ROOMESCAPEFPS_API AGhostSoul : public APawn, public IFreelistObjectInterface
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AGhostSoul();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	class UBoxComponent* GetBoundingBox();

	virtual bool IsInFreeList() override;
	virtual void SetIsInFreeList(bool bFreeList) override;

	UFUNCTION()
		void SetAsDead();

	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		void DelayActivateParticle();

	UPROPERTY()
		bool bIsInFreeList;

private:
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* BodyMesh;

	UPROPERTY(EditAnywhere, Category = Collision, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* SphereCol;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* MoveToLocationBoundingBox;

	UPROPERTY(EditAnywhere, Category = Particle, meta = (AllowPrivateAccess = "true"))
		class UParticleSystemComponent* GhostParticle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UFloatingPawnMovement* GhostMovementComponent;

	UPROPERTY(/*Replicated, */EditAnywhere, BlueprintReadOnly, Category = "Ghost Col Size", Meta = (AllowPrivateAccess = "true"))
		float SphereRadius;

	UPROPERTY(/*Replicated, */EditAnywhere, BlueprintReadOnly, Category = "Ghost Movement Target Pos", Meta = (AllowPrivateAccess = "true"))
		FVector BonudingBoxSize;

	FTimerHandle EmitterDelayTimer;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "GameFramework/Pawn.h"
#include "GhostSoul.generated.h"

class UFloatingPawnMovement;
class USphereComponent;
class UBoxComponent;
/**
 * 생성: GhostSpawner->GhostFreelist 이후 GhostAIController에서 액터 제어
 * 소멸(Deactive): Ghost(피격 판정받음)->GhostAIController(사라짐 상태 변경)->
 *					Ghost(사라짐 연출 후 종료 델리게이트)->GhostSpawner(오브젝트 반납)->GhostFreelist
 */
UCLASS()
class ROOMESCAPEFPS_API AGhostSoul : public APawn
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AGhostSoul();

#if WITH_EDITOR
	void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UBoxComponent* GetBoundingBox();

	void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;

private:
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> BodyMesh;

	UPROPERTY(EditAnywhere, Category = Collision, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> SphereCol;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> MoveToLocationBoundingBox;

	UPROPERTY()
	TObjectPtr<UNiagaraComponent> GhostParticle;

	UPROPERTY(EditAnywhere, Category = Niagara, meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<UNiagaraSystem> GhostParticleObj;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UFloatingPawnMovement> GhostMovementComponent;

	UPROPERTY(/*Replicated, */EditAnywhere, BlueprintReadOnly, Category = "Ghost Col Size", Meta = (AllowPrivateAccess = "true"))
	float SphereRadius;

	UPROPERTY(/*Replicated, */EditAnywhere, BlueprintReadOnly, Category = "Ghost Movement Target Pos", Meta = (AllowPrivateAccess = "true"))
	FVector BonudingBoxSize;

	FTimerHandle EmitterDelayTimer;
};

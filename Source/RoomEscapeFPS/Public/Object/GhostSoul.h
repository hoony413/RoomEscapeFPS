// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/FreelistObjectInterface.h"
#include "GameFramework/Pawn.h"
#include "GhostSoul.generated.h"

/**
 * ����: GhostSpawner->GhostFreelist ���� GhostAIController���� ���� ����
 * �Ҹ�(Deactive): Ghost(�ǰ� ��������)->GhostAIController(����� ���� ����)->
 *					Ghost(����� ���� �� ���� ��������Ʈ)->GhostSpawner(������Ʈ �ݳ�->GhostFreelist
 */
UCLASS()
class ROOMESCAPEFPS_API AGhostSoul : public APawn, public IFreelistObjectInterface
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AGhostSoul();

	void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	class UBoxComponent* GetBoundingBox();

	virtual bool IsInFreeList() override;
	virtual void SetIsInFreeList(bool bFreeList) override;

	UFUNCTION()
		void PlayDeadAnimation();

	virtual void Tick(float DeltaTime) override;

protected:
	UFUNCTION(Server, Reliable)
		void ServerReturnGhostElement();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
		bool bIsInFreeList;

private:
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* BodyMesh;

	UPROPERTY(EditAnywhere, Category = Collision, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* SphereCol;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* MoveToLocationBoundingBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UFloatingPawnMovement* GhostMovementComponent;

	UPROPERTY(/*Replicated, */EditAnywhere, BlueprintReadOnly, Category = "Ghost Col Size", Meta = (AllowPrivateAccess = "true"))
		float SphereRadius;

	UPROPERTY(/*Replicated, */EditAnywhere, BlueprintReadOnly, Category = "Ghost Movement Target Pos", Meta = (AllowPrivateAccess = "true"))
		FVector BonudingBoxSize;

	UPROPERTY(EditAnywhere, Category = "Ghost Dead Mat", Meta = (AllowPrivateAccess = "true"))
		class UMaterialInstance* DeadMaterialInstance;

	UPROPERTY()
		class UMaterialInstanceDynamic* mid;
	UPROPERTY()
		class UMaterialInstance* defaultMat;

	float fDeadAnimStartTime = 0.f;

	bool bMarkDead = false;
};

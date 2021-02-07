// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/FreelistActor.h"
#include "GhostSoul.generated.h"

/**
 * 
 */
UCLASS()
class ROOMESCAPEFPS_API AGhostSoul : public AFreelistActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AGhostSoul();

	void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UStaticMeshComponent* BodyMesh;

	UPROPERTY(EditAnywhere, Category = Collision)
		class USphereComponent* SphereCol;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Ghost Col Size", Meta = (AllowPrivateAccess = "true"))
		float SphereRadius;
};

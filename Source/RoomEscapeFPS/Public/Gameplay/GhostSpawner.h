// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GhostSpawner.generated.h"

class AGhostSoul;
class UBoxComponent;

UCLASS()
class ROOMESCAPEFPS_API AGhostSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGhostSpawner();

	void SpawnGhost();
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;

public:	
	// Called every frame
	void Tick(float DeltaTime) override;

	void SetActive(bool bInActive);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> SpawnVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSoftClassPtr<AGhostSoul> GhostClass;

	UPROPERTY(Transient)
	TArray<TObjectPtr<AGhostSoul>> ActiveGhosts;
	
	UPROPERTY(Replicated)
	bool bActive;

	FTimerHandle SpawnTimer;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float fSpawnTime = 1.5f;
};

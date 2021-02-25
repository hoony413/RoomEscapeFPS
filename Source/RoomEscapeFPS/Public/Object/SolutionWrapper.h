// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gameplay/TypeInfoHeader.h"
#include "SolutionWrapper.generated.h"

UCLASS()
class ROOMESCAPEFPS_API ASolutionWrapper : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASolutionWrapper();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable)
		void ServerOnStateChanged();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY()
		class USceneComponent* ParentComp;
	UPROPERTY()
		TArray<class UChildActorComponent*> ChildActors;

private:
	UPROPERTY(EditAnywhere, Category = "Solution Info", meta = (AllowPrivateAccess = "true"))
		EServerSolutionType SolutionType;
};

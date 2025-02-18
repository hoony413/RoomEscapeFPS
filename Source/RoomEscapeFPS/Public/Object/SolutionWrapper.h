// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gameplay/TypeInfoHeader.h"
#include "SolutionWrapper.generated.h"

/*
 * 문제 풀이 시, 차일드 액터들이 변화되었음을 통지 받아 서버에 정답 제출을 돕는 객체.
 * 차일드 액터의 OnInteractionHappened 델리게이트에 ServerOnStateChanged()를 연결한다.
 */
UCLASS()
class ROOMESCAPEFPS_API ASolutionWrapper : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASolutionWrapper();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

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
	UPROPERTY(Replicated)
		bool bIsCleared;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "InteractiveObject.generated.h"

class UBoxComponent;

UENUM()
enum class EInteractiveObjectState
{
	EState_Open_Or_On,
	EState_Playing,
	EState_Close_Or_Off,
};
UCLASS(Blueprintable)
class ROOMESCAPEFPS_API AInteractiveObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractiveObject();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	UFUNCTION()
	virtual void OnInteraction(EInteractiveObjectState NextState);

	UFUNCTION()
		virtual void OnRep_CurrentState();
	EInteractiveObjectState GetCurrentState() { return CurrentState; }
	virtual EInteractiveObjectState GetNextState();

	virtual void ToggleState(APawn* requester);

	FORCEINLINE const FString& GetInformationMessage() const { return InformationStr; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// 오브젝트 브로드캐스트
	UFUNCTION(NetMulticast, Unreliable)
		void NetMulticast_Interaction(EInteractiveObjectState NextState);

protected:
	virtual void SetTimeline();
	// UFUNCTION 상속을 위한 함수 분리
	UFUNCTION()
		void LaunchTimeline() { LaunchTimelineInternal(); }
	virtual void LaunchTimelineInternal();

protected:
	UPROPERTY(VisibleDefaultsOnly)
		UBoxComponent* LineTraceBox;
	UPROPERTY(EditAnywhere)
		FVector LineTraceBoxOffset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector LineTraceBoxSize;

	UPROPERTY(EditAnywhere, Category = "Timeline Info")
		bool IsUseTimeline = false;
	UPROPERTY(EditAnywhere, Category = "Timeline Info")
		float CurveWeightValue = 1.f;
	UPROPERTY(EditAnywhere, Category = "Timeline Info")
		class UStaticMeshComponent* TimelineMesh;
	UPROPERTY(EditAnywhere, Category = "Timeline Info")
		class UCurveFloat* TimelineCurve;
	UPROPERTY()
		FTimeline Timeline;
	
	UPROPERTY(EditAnywhere, Category = "Information Message")
		FString InformationStr;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentState)
		EInteractiveObjectState	CurrentState = EInteractiveObjectState::EState_Close_Or_Off;
	
	float TimelineDelta = 0.f;
	float CurveFloatValue = 0.f;

};

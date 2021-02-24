// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "Gameplay/TypeInfoHeader.h"
#include "InteractiveObject.generated.h"

class UBoxComponent;

UENUM()
enum class EInteractiveObjectState : uint8
{
	EState_Open_Or_On,
	EState_Close_Or_Off,
};

UENUM()
enum class ETimelineControlType : uint8
{
	ELocationX,
	ELocationY,
	ELocationZ,
	ERotationX,
	ERotationY,
	ERotationZ,
};

USTRUCT()
struct ROOMESCAPEFPS_API FTimelineInfo
{
	GENERATED_BODY()

public:
	FTimelineInfo() 
	{
		CurrentState = EInteractiveObjectState::EState_Close_Or_Off;
		StaticMeshComponent = nullptr;
		fCurrentCurveValue = 0.f;
		fTimelineDelta = 0.f;
	}
	//FTimelineInfo(int32 InIndex, ETimelineControlType InType, EInteractiveObjectState InState = EInteractiveObjectState::EState_Close_Or_Off, float InCurveWeightValue = 1.f)
	//{
	//	ControlType = InType;
	//	CurrentState = InState;
	//	fCurveWeightValue = InCurveWeightValue;
	//
	//	fCurrentCurveValue = 0.f;
	//	fTimelineDelta = 0.f;
	//}

	
	UPROPERTY(EditAnywhere)
		EInteractiveObjectState CurrentState;
	UPROPERTY(EditAnywhere)
		float fCurveWeightValue;
	UPROPERTY(EditAnywhere)
		ETimelineControlType ControlType;

	UPROPERTY()
		class UStaticMeshComponent* StaticMeshComponent;
	UPROPERTY()
		float fCurrentCurveValue;
	UPROPERTY()
		float fTimelineDelta;
	UPROPERTY()
		FTimeline Timeline;
};

DECLARE_DELEGATE(FOnChildObjectChangedDele);

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
	virtual void OnInteraction(APawn* requester, class UPrimitiveComponent* InComp);

	FORCEINLINE const FString& GetInformationMessage() const { return InformationStr; }

	UFUNCTION()
		FORCEINLINE bool IsNotInteractive() { return bIsNonInteractive; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 타임라인 애니메이션 멀티캐스트
	UFUNCTION(NetMulticast, Unreliable)
		virtual void NetMulticast_Timeline(int32 index, EInteractiveObjectState InState);

protected:
	virtual void SetTimeline();
	
	FTimelineInfo* FindTimelineMeshComponent(class UStaticMeshComponent* InMesh, int32& OutIndex);

public:
	FOnChildObjectChangedDele OnChildObjectChanged;

protected:
	UPROPERTY(EditAnywhere)
		class USceneComponent* ParentComp;

	UPROPERTY(EditAnywhere)
		UBoxComponent* LineTraceBox;
	UPROPERTY(EditAnywhere)
		FVector LineTraceBoxOffset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector LineTraceBoxSize;

	UPROPERTY(EditAnywhere, Category = "Information Message")
		FString InformationStr;

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* DefaultMesh;

	UPROPERTY(EditAnywhere)
		bool bIsNonInteractive;

	UPROPERTY(Replicated, EditAnywhere, Category = "Timeline Info", meta = (AllowPrivateAccess = "true"))
		TArray<FTimelineInfo> TimelineMeshes;

	float StartCurveValue = 0.f;

private:
	UPROPERTY(EditAnywhere, Category = "Timeline Info", meta = (AllowPrivateAccess = "true"))
		bool IsUseTimeline = false;
	UPROPERTY(EditAnywhere, Category = "Timeline Info", meta = (AllowPrivateAccess = "true"))
		class UCurveFloat* TimelineCurve;

	float TimelineDelta = 0.f;
	float CurveFloatValue = 0.f;
};

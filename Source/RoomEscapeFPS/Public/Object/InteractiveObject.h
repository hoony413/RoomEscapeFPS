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
	NONE = 0 UMETA(Hidden),
	STATE_OPEN_OR_ON,
	STATE_CLOSE_OR_OFF,
	MAX UMETA(Hidden)
};

UENUM()
enum class ETimelineControlType : uint8
{
	NONE = 0 UMETA(Hidden),
	LOCATION_X,
	LOCATION_Y,
	LOCATION_Z,
	ROTATION_X,
	ROTATION_Y,
	ROTATION_Z,
	MAX UMETA(Hidden)
};

USTRUCT()
struct ROOMESCAPEFPS_API FTimelineInfo
{
	GENERATED_BODY()

public:
	FTimelineInfo() 
	{
		CurrentState = EInteractiveObjectState::STATE_CLOSE_OR_OFF;
		StaticMeshComponent = nullptr;
		fCurrentCurveValue = 0.f;
		fTimelineDelta = 0.f;
	}
	
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

DECLARE_DELEGATE(FOnInteractionHappened);
DECLARE_DELEGATE_RetVal_TwoParams(bool, FOnSolutionSuccessResult, APawn*, UPrimitiveComponent*)

/*
 * 상호작용 오브젝트의 최상위 클래스. 서버에서 호출되는 OnInteraction 함수를 재정의 가능하며, 
 * StaticMesh들의 간단한 애니메이션을 위한 구조체인 FTimelineInfo 구조체를 소유한다.
 * OnInteractionHappened 델리게이트를 통해 다른 객체에게 메시지를 전달할 수 있다.
 * 상호작용 불가능 플래그, 상호작용 안내 메시지(UI) 스트링을 BP에서 설정할 수 있다.
 */

UCLASS(Blueprintable)
class ROOMESCAPEFPS_API AInteractiveObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractiveObject();
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	// Called every frame
	void Tick(float DeltaTime) override;

#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	UFUNCTION()
	virtual bool OnInteraction(APawn* requester, UPrimitiveComponent* InComp);

	FORCEINLINE const FString& GetInformationMessage() const { return InformationStr; }
	FORCEINLINE UPrimitiveComponent* GetSolutionResultComp() const { return cachedSolutionResultComp; }

	FORCEINLINE bool IsNonInteractable() const { return bIsNonInteractable; }

	FORCEINLINE EServerSolutionResultType GetSolutionResultType() const { return SolutionResultType; }

protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;

	// 타임라인 애니메이션 멀티캐스트
	UFUNCTION(NetMulticast, Reliable)
	virtual void NetMulticast_Timeline(int32 index, EInteractiveObjectState InState);
	virtual void SetTimeline();
	FTimelineInfo* FindTimelineMeshComponent(class UStaticMeshComponent* InMesh, int32& OutIndex);

public:
	FOnInteractionHappened OnInteractionHappened;
	FOnSolutionSuccessResult OnSolutionSuccessResult;

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> ParentComp;

	UPROPERTY(EditAnywhere)
	UBoxComponent* LineTraceBox;
	UPROPERTY(EditAnywhere)
	FVector LineTraceBoxOffset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector LineTraceBoxSize;

	UPROPERTY(EditAnywhere, Category = "Information Message")
	FString InformationStr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> DefaultMesh;

	// 상호작용 가능 여부(단순히 장식용으로 월드에 배치된 경우 BP에서 설정)
	UPROPERTY(Replicated, EditAnywhere)
	bool bIsNonInteractable;
	// 문제 풀이의 결과로 발동되는 오브젝트(Server Interactable Only인지. 기본값: NONE)
	UPROPERTY(Replicated, EditAnywhere)
	EServerSolutionResultType SolutionResultType;
	// 타임라인 상태 동기화(문이 열렸는지, 닫혔는지가 동기화되어야 함)
	UPROPERTY(Replicated, EditAnywhere, Category = "Timeline Info", meta = (AllowPrivateAccess = "true"))
	TArray<FTimelineInfo> TimelineMeshes;

	UPROPERTY()
	TObjectPtr<UPrimitiveComponent> cachedSolutionResultComp;

	float StartCurveValue = 0.f;

private:
	UPROPERTY(EditAnywhere, Category = "Timeline Info", meta = (AllowPrivateAccess = "true"))
		bool IsUseTimeline = false;
	UPROPERTY(EditAnywhere, Category = "Timeline Info", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCurveFloat> TimelineCurve;

	float TimelineDelta = 0.f;
	float CurveFloatValue = 0.f;
};

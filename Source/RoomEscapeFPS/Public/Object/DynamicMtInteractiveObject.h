// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/InteractiveObject.h"
#include "DynamicMtInteractiveObject.generated.h"

/**
 * Interaction과 함께 DynamicMaterial 제어를 위한 클래스.
 */
UENUM()
enum class EDynamicMtParamType : uint8
{
	NONE = 0 UMETA(Hidden),
	SCALAR,
	VECTOR,
	MAX UMETA(Hidden)
};

USTRUCT()
struct ROOMESCAPEFPS_API FDynamicMtInfo
{
	GENERATED_BODY()
public:
	FDynamicMtInfo() = default;

	UPROPERTY(EditAnywhere)
	FName ParameterName;
	UPROPERTY(EditAnywhere)
	EDynamicMtParamType ParameterType;
	UPROPERTY(EditAnywhere)
	FFloatInterval ParameterValue;
	UPROPERTY(EditAnywhere)
	FTwoVectors ParameterVector;
	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DynamicMt;
};

UCLASS()
class ROOMESCAPEFPS_API ADynamicMtInteractiveObject : public AInteractiveObject
{
	GENERATED_BODY()

public:
	bool OnInteraction(APawn* requester, class UPrimitiveComponent* InComp) override;

protected:
	void BeginPlay() override;

	FDynamicMtInfo* FindDynamicMtMeshComponent(class UStaticMeshComponent* InMesh, int32& OutIndex);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_DynamicMaterial(int32 MtIndex, EInteractiveObjectState InState);

protected:
	UPROPERTY(EditAnywhere, Category = "DynamicMaterial Info", meta = (AllowPrivateAccess = "true"))
	TArray<FDynamicMtInfo> DynamicMtArray;
};

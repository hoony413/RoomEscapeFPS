// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/InteractiveObject.h"
#include "DynamicMtInteractiveObject.generated.h"

/**
 * 
 */
UENUM()
enum class EDynamicMtParamType : uint8
{
	EScalar,
	EVector,
};

USTRUCT()
struct ROOMESCAPEFPS_API FDynamicMtInfo
{
	GENERATED_BODY()
public:
	FDynamicMtInfo() 
	{
		StaticMeshComponent = nullptr;
		DynamicMt = nullptr;
	}

	UPROPERTY(EditAnywhere)
		FName ParameterName;
	UPROPERTY(EditAnywhere)
		EDynamicMtParamType ParameterType;
	UPROPERTY(EditAnywhere)
		FFloatInterval ParameterValue;
	UPROPERTY(EditAnywhere)
		FTwoVectors ParameterVector;
	UPROPERTY()
		class UStaticMeshComponent* StaticMeshComponent;
	UPROPERTY()
		class UMaterialInstanceDynamic* DynamicMt;
};

UCLASS()
class ROOMESCAPEFPS_API ADynamicMtInteractiveObject : public AInteractiveObject
{
	GENERATED_BODY()

public:
	virtual void OnInteraction(APawn* requester, class UPrimitiveComponent* InComp) override;

protected:
	virtual void BeginPlay() override;

	FDynamicMtInfo* FindDynamicMtMeshComponent(class UStaticMeshComponent* InMesh, int32& OutIndex);

	UFUNCTION(NetMulticast, Unreliable)
		virtual void NetMulticast_DynamicMaterial(int32 MtIndex, EInteractiveObjectState InState);

protected:
	UPROPERTY(EditAnywhere, Category = "DynamicMaterial Info", meta = (AllowPrivateAccess = "true"))
		TArray<FDynamicMtInfo> DynamicMtArray;
};

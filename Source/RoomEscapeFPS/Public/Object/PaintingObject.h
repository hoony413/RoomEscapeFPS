// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/InteractiveObject.h"
#include "PaintingObject.generated.h"

/**
 * 돌릴 수 있는 그림 오브젝트.
 */
UENUM()
enum class ERotateState : uint8
{
	ERotate_0,
	ERotate_90,
	ERotate_180,
	ERotate_270,
};

UCLASS()
class ROOMESCAPEFPS_API APaintingObject : public AInteractiveObject
{
	GENERATED_BODY()
	
public:
	APaintingObject();

	virtual bool OnInteraction(APawn* requester, class UPrimitiveComponent* InComp) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	FORCEINLINE ERotateState GetRotateState() { return RotateState; }
	FORCEINLINE void SetRotateState(ERotateState InState) { RotateState = InState; }

	FORCEINLINE int32 GetDigit() { return Digit; }
	FORCEINLINE void SetDigit(int32 InDigit) { Digit = InDigit; }

	virtual void NetMulticast_Timeline_Implementation(int32 index, EInteractiveObjectState InState) override;


private:
	UPROPERTY(Replicated)
		ERotateState RotateState;

	UPROPERTY(Replicated)
		int32 Digit;
};

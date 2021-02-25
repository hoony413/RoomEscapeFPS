// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/DynamicMtInteractiveObject.h"
#include "SwitchButtonObject.generated.h"

/**
 * 
 */
UCLASS()
class ROOMESCAPEFPS_API ASwitchButtonObject : public ADynamicMtInteractiveObject
{
	GENERATED_BODY()
	
public:
	ASwitchButtonObject();

	virtual bool OnInteraction(APawn* requester, class UPrimitiveComponent* InComp) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	FORCEINLINE bool GetIsPressed() { return bSwitchPressed; }

	FORCEINLINE void SetDigit(int32 InNum) { Digit = InNum; }
	FORCEINLINE int32 GetDigit() { return Digit; }

private:
	UPROPERTY(Replicated)
		bool bSwitchPressed;
	UPROPERTY(Replicated)
		int32 Digit;
};

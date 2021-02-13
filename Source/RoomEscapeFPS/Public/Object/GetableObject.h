// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/InteractiveObject.h"
#include "Gameplay/TypeInfoHeader.h"
#include "GetableObject.generated.h"

/**
 * 
 */
UCLASS()
class ROOMESCAPEFPS_API AGetableObject : public AInteractiveObject
{
	GENERATED_BODY()

public:
	AGetableObject();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void ToggleState(APawn* requester) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = ItemInfo, meta = (AllowPrivateAccess = "true"))
		EItemType ItemType;
	UPROPERTY(EditAnywhere, Category = ItemInfo, meta = (AllowPrivateAccess = "true", UIMin = "1"))
		int32 DefaultGetCount;
};

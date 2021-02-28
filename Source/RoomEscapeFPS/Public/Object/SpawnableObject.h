// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/InteractiveObject.h"
#include "SpawnableObject.generated.h"

/**
 * 배터리, 플래시, 열쇠, 부적 등의 액터를 내부에서 스폰 가능한 오브젝트.
 */
UCLASS()
class ROOMESCAPEFPS_API ASpawnableObject : public AInteractiveObject
{
	GENERATED_BODY()
	
public:
	ASpawnableObject() {}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		TSoftClassPtr<class AGetableObject> FlashObj;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		TSoftClassPtr<class AGetableObject> BatteryObj;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		TSoftClassPtr<class AGetableObject> CharmObj;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		TSoftClassPtr<class AGetableObject> KeyObj;
};

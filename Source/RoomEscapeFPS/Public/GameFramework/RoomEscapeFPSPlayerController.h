// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RoomEscapeFPSPlayerController.generated.h"

/**
 * 
 */

UCLASS()
class ROOMESCAPEFPS_API ARoomEscapeFPSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void SetupInputComponent() override;

	UFUNCTION()
		void OnTestKey();

protected:
	UFUNCTION()
	void Jump();
	UFUNCTION()
	void StopJumping();
	UFUNCTION()
	void OnFlash();
	UFUNCTION()
	void OnUse();
	UFUNCTION()
	void OnFire();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	void AddControllerYawInput(float Val);
	void AddControllerPitchInput(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	UFUNCTION(Server, Reliable)
		void ServerOnTestKey();

	UFUNCTION(Client, Reliable)
		void ClientOnTestKey();

public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

};

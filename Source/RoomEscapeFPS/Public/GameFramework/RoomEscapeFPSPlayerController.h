// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Gameplay/TypeInfoHeader.h"
#include "InputActionValue.h"
#include "RoomEscapeFPSPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class AGetableObject;

/**
 *
 */

UCLASS()
class ROOMESCAPEFPS_API ARoomEscapeFPSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	void SetupInputComponent() override;

	UFUNCTION()
void OnTestKey();

	UFUNCTION(BlueprintPure, Category = "Input")
	UInputAction* GetUseAction() const { return _useAction; }

	UFUNCTION(Client, Reliable)
void ClientSetupHUD();

protected:
	void OnPossess(APawn* InPawn) override;
	void OnUnPossess() override;
	void AcknowledgePossession(APawn* P) override;

	void HandleJump(FInputActionValue const& value);
	void HandleStopJumping(FInputActionValue const& value);
	void HandleFlash(FInputActionValue const& value);
	void HandleUse(FInputActionValue const& value);
	void HandleFire(FInputActionValue const& value);
	void HandleMove(FInputActionValue const& value);
	void HandleLook(FInputActionValue const& value);
	void HandleTestKey(FInputActionValue const& value);

	UFUNCTION(Server, Reliable)
void ServerOnTestKey();

	UFUNCTION(Client, Reliable)
	void ClientOnTestKey();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> _defaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> _jumpAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> _flashAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> _useAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> _fireAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> _testKeyAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> _moveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> _lookAction;

public:
	// 파이프 게임 RPC
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRotatePipe(int32 Index);

	UFUNCTION(Server, Reliable)
	void ServerCheckCommittedAnswer();

	UFUNCTION(Server, Reliable)
	void ServerClearPipeGame();

	UFUNCTION(Client, Reliable)
	void ClientProcessHUDOnFirstItemGet(AGetableObject* InObj);

	UFUNCTION(Client, Reliable)
	void ClientProcessHUDOnUpdateNextInfo(ENextInformationType curType, ENextInformationType nextType, int32 InCount);
};

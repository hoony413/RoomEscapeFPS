// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/RoomEscapeFPSPlayerController.h"
#include "Helper/Helper.h"
#include "Managers/UIManager.h"
#include "UI/BasePage.h"
#include "GameFramework/RoomEscapeFPSGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
//#include "RoomEscapeFPS/RoomEscapeFPSGameState.h"
#include "Character/RoomEscapeFPSCharacter.h"

void ARoomEscapeFPSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	InputComponent->BindAction("Jump", IE_Pressed, this, &ARoomEscapeFPSPlayerController::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ARoomEscapeFPSPlayerController::StopJumping);

	// Bind Flash event
	InputComponent->BindAction("Flash", IE_Pressed, this, &ARoomEscapeFPSPlayerController::OnFlash);

	// Bind Use event
	InputComponent->BindAction("Use", IE_Pressed, this, &ARoomEscapeFPSPlayerController::OnUse);

	InputComponent->BindAction("TestKey", IE_Pressed, this, &ARoomEscapeFPSPlayerController::OnTestKey);
	
	// Bind movement events
	InputComponent->BindAxis("MoveForward", this, &ARoomEscapeFPSPlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ARoomEscapeFPSPlayerController::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &ARoomEscapeFPSPlayerController::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &ARoomEscapeFPSPlayerController::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &ARoomEscapeFPSPlayerController::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &ARoomEscapeFPSPlayerController::LookUpAtRate);
}
void ARoomEscapeFPSPlayerController::Jump()
{
	ACharacter* character = GetCharacter();
	if (character)
	{
		character->Jump();
	}
}
void ARoomEscapeFPSPlayerController::StopJumping()
{
	ACharacter* character = GetCharacter();
	if (character)
	{
		character->StopJumping();
	}
}

void ARoomEscapeFPSPlayerController::OnFlash()
{
	ARoomEscapeFPSCharacter* character = Cast<ARoomEscapeFPSCharacter>(GetCharacter());
	if (character)
	{
		character->OnFlash();
	}
}
void ARoomEscapeFPSPlayerController::OnUse()
{
	ARoomEscapeFPSCharacter* character = Cast<ARoomEscapeFPSCharacter>(GetCharacter());
	if (character)
	{
		character->OnUse();
	}
}
void ARoomEscapeFPSPlayerController::OnTestKey()
{
	// 개인 UI 조작 시 이런식으로 호출하면 된다.
	//GetWorld()->GetAuthGameMode();
	// 서버 RPC로 랩핑해서 해보자.
	//GetUIMgr()->ShowWidget<UBasePage>(GetPawn());
	ServerOnTestKey();
}
void ARoomEscapeFPSPlayerController::ServerOnTestKey_Implementation()
{
	if (HasAuthority())
	{
		AGameModeBase* gMode = GetWorld()->GetAuthGameMode();
		if (gMode)
		{
			ARoomEscapeFPSGameMode* g = Cast<ARoomEscapeFPSGameMode>(gMode);
			if (g)
			{
				const TArray<APlayerState*>& arr = g->GameState->PlayerArray;
				for (const auto& elem : arr)
				{
					int32 id = elem->GetPlayerId();
					g->InitializePipeGame(id, 2);
				}
			}
		}
	}
}
void ARoomEscapeFPSPlayerController::AddControllerYawInput(float Value)
{
	APawn* character = GetPawn();
	if (character)
	{
		character->AddControllerYawInput(Value);
	}
}
void ARoomEscapeFPSPlayerController::AddControllerPitchInput(float Value)
{
	APawn* character = GetPawn();
	if (character)
	{
		character->AddControllerPitchInput(Value);
	}
}
void ARoomEscapeFPSPlayerController::MoveForward(float Value)
{
	APawn* pawn = GetPawn();
	if (pawn && Value != 0.0f)
	{
		// add movement in that direction
		pawn->AddMovementInput(pawn->GetActorForwardVector(), Value);
	}
}
void ARoomEscapeFPSPlayerController::MoveRight(float Value)
{
	APawn* pawn = GetPawn();
	if (pawn && Value != 0.0f)
	{
		// add movement in that direction
		pawn->AddMovementInput(pawn->GetActorRightVector(), Value);
	}
}
void ARoomEscapeFPSPlayerController::TurnAtRate(float Rate)
{
	APawn* pawn = GetPawn();
	if (pawn)
	{
		// calculate delta for this frame from the rate information
		pawn->AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	}
}
void ARoomEscapeFPSPlayerController::LookUpAtRate(float Rate)
{
	APawn* pawn = GetPawn();
	if (pawn)
	{
		// calculate delta for this frame from the rate information
		pawn->AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	}
	
}
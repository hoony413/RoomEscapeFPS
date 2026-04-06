// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/RoomEscapeFPSPlayerController.h"
#include "GameFramework/RoomEscapeFPSGameMode.h"
#include "GameFramework/RoomEscapeFPSHUD.h"
#include "GameFramework/RoomEscapeFPSPlayerState.h"
#include "GameFramework/RoomEscapeFPSGameState.h"
#include "Character/RoomEscapeFPSCharacter.h"
#include "Object/GetableObject.h"
#include "Net/UnrealNetwork.h"

#include "Helper/Helper.h"
#include "Managers/UISubsystem.h"
#include "UI/PipeGamePanel.h"
#include "UI/FirstGetItemInfoPanel.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


void ARoomEscapeFPSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* enhancedInput = Cast<UEnhancedInputComponent>(InputComponent);
	if (not enhancedInput)
	{
		return;
	}

	enhancedInput->BindAction(_jumpAction,    ETriggerEvent::Started,   this, &ThisClass::HandleJump);
	enhancedInput->BindAction(_jumpAction,    ETriggerEvent::Completed, this, &ThisClass::HandleStopJumping);
	enhancedInput->BindAction(_flashAction,   ETriggerEvent::Started,   this, &ThisClass::HandleFlash);
	enhancedInput->BindAction(_useAction,     ETriggerEvent::Started,   this, &ThisClass::HandleUse);
	enhancedInput->BindAction(_fireAction,    ETriggerEvent::Started,   this, &ThisClass::HandleFire);
	enhancedInput->BindAction(_testKeyAction, ETriggerEvent::Started,   this, &ThisClass::HandleTestKey);
	enhancedInput->BindAction(_moveAction,    ETriggerEvent::Triggered,  this, &ThisClass::HandleMove);
	enhancedInput->BindAction(_lookAction,    ETriggerEvent::Triggered,  this, &ThisClass::HandleLook);
}

void ARoomEscapeFPSPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void ARoomEscapeFPSPlayerController::AcknowledgePossession(APawn* P)
{
	Super::AcknowledgePossession(P);

	ULocalPlayer const* localPlayer = GetLocalPlayer();
	if (not localPlayer)
	{
		return;
	}

	if (UEnhancedInputLocalPlayerSubsystem* subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(localPlayer))
	{
		if (_defaultMappingContext)
		{
			subsystem->AddMappingContext(_defaultMappingContext, 0);
		}
	}
}

void ARoomEscapeFPSPlayerController::OnUnPossess()
{
	if (ULocalPlayer const* localPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(localPlayer))
		{
			if (_defaultMappingContext)
			{
				subsystem->RemoveMappingContext(_defaultMappingContext);
			}
		}
	}

	Super::OnUnPossess();
}

void ARoomEscapeFPSPlayerController::ClientSetupHUD_Implementation()
{
	if (UUISubsystem* uiSubsystem = Helper::GetSubsystem<UUISubsystem>(GetWorld()))
	{
		uiSubsystem->HideLoadingScreen();
	}

	if (ARoomEscapeFPSHUD* hud = Cast<ARoomEscapeFPSHUD>(GetHUD()))
	{
		hud->InitializeHUD();
	}
}

void ARoomEscapeFPSPlayerController::HandleJump(FInputActionValue const& value)
{
	if (ACharacter* character = GetCharacter())
	{
		character->Jump();
	}
}

void ARoomEscapeFPSPlayerController::HandleStopJumping(FInputActionValue const& value)
{
	if (ACharacter* character = GetCharacter())
	{
		character->StopJumping();
	}
}

void ARoomEscapeFPSPlayerController::HandleFlash(FInputActionValue const& value)
{
	if (ARoomEscapeFPSCharacter* character = Cast<ARoomEscapeFPSCharacter>(GetCharacter()))
	{
		character->OnFlash();
	}
}

void ARoomEscapeFPSPlayerController::HandleUse(FInputActionValue const& value)
{
	if (ARoomEscapeFPSCharacter* character = Cast<ARoomEscapeFPSCharacter>(GetCharacter()))
	{
		character->OnUse();
	}
}

void ARoomEscapeFPSPlayerController::HandleFire(FInputActionValue const& value)
{
	if (ARoomEscapeFPSCharacter* character = Cast<ARoomEscapeFPSCharacter>(GetCharacter()))
	{
		character->OnFire();
	}
}

void ARoomEscapeFPSPlayerController::HandleTestKey(FInputActionValue const& value)
{
}

void ARoomEscapeFPSPlayerController::HandleMove(FInputActionValue const& value)
{
	APawn* pawn = GetPawn();
	if (not pawn)
	{
		return;
	}

	FVector2D const moveVector = value.Get<FVector2D>();

	if (0.0f != moveVector.Y)
	{
		pawn->AddMovementInput(pawn->GetActorForwardVector(), moveVector.Y);
	}

	if (0.0f != moveVector.X)
	{
		pawn->AddMovementInput(pawn->GetActorRightVector(), moveVector.X);
	}
}

void ARoomEscapeFPSPlayerController::HandleLook(FInputActionValue const& value)
{
	APawn* pawn = GetPawn();
	if (not pawn)
	{
		return;
	}

	FVector2D const lookVector = value.Get<FVector2D>();

	if (0.0f != lookVector.X)
	{
		pawn->AddControllerYawInput(lookVector.X);
	}

	if (0.0f != lookVector.Y)
	{
		pawn->AddControllerPitchInput(lookVector.Y);
	}
}

void ARoomEscapeFPSPlayerController::OnTestKey()
{
	// 개인 UI 조작 시 이런식으로 호출하면 된다.
	//ServerOnTestKey();
}

void ARoomEscapeFPSPlayerController::ServerOnTestKey_Implementation()
{
	//if (GetNetMode() == NM_DedicatedServer)
	//{
	//	ARoomEscapeFPSGameMode* gm = GetWorld()->GetAuthGameMode<ARoomEscapeFPSGameMode>();
	//	if (gm)
	//	{
	//		ARoomEscapeFPSGameState* gs = gm->GetGameState<ARoomEscapeFPSGameState>();
	//		if (gs)
	//		{
	//			ARoomEscapeFPSPlayerState* ps = GetPlayerState<ARoomEscapeFPSPlayerState>();
	//			if (ps)
	//			{
	//				int32 id = ps->GetPlayerId();
	//				for (auto& elem : gs->PlayerArray)
	//				{
	//					ARoomEscapeFPSPlayerState* gsps = Cast<ARoomEscapeFPSPlayerState>(elem);
	//					if (gsps->GetPlayerId() == id)
	//					{
	//						gsps->InitializePipeGame(5);
	//					}
	//				}
	//			}
	//		}
	//	}
	//}
}

void ARoomEscapeFPSPlayerController::ClientOnTestKey_Implementation()
{
}

bool ARoomEscapeFPSPlayerController::ServerRotatePipe_Validate(int32 Index)
{
	if (ARoomEscapeFPSPlayerState const* ps = GetPlayerState<ARoomEscapeFPSPlayerState>())
	{
		return 0 <= Index && Index < ps->GetPipeGameInfo().GetPipeNodes().Num();
	}
	return false;
}
void ARoomEscapeFPSPlayerController::ServerRotatePipe_Implementation(int32 Index)
{
	if (ARoomEscapeFPSPlayerState* ps = GetPlayerState<ARoomEscapeFPSPlayerState>())
	{
		ps->RotatePipe(Index);
	}
}
void ARoomEscapeFPSPlayerController::ServerCheckCommittedAnswer_Implementation()
{
	if (ARoomEscapeFPSPlayerState* ps = GetPlayerState<ARoomEscapeFPSPlayerState>())
	{
		ps->CheckAndApplyPipeAnswer();
	}
}
void ARoomEscapeFPSPlayerController::ServerClearPipeGame_Implementation()
{
	if (ARoomEscapeFPSPlayerState* ps = GetPlayerState<ARoomEscapeFPSPlayerState>())
	{
		ps->ClearPipeGame();
	}
}
void ARoomEscapeFPSPlayerController::ClientProcessHUDOnFirstItemGet_Implementation(AGetableObject* InObj)
{
	UUISubsystem* uiSubsystem = Helper::GetSubsystem<UUISubsystem>(GetWorld());
	if (not uiSubsystem)
	{
		return;
	}
	if (UFirstGetItemInfoPanel* ItemInfoUI = uiSubsystem->OpenWidget<UFirstGetItemInfoPanel>(EActivatableWidgetType::FIRST_GET_ITEM_INFO))
	{
		ItemInfoUI->SetItemNameText(InObj->GetItemNameStr());
		ItemInfoUI->SetItemDescText(InObj->GetItemDescStr());
	}
	ARoomEscapeFPSHUD* hud = Cast<ARoomEscapeFPSHUD>(GetHUD());
	if (hud && InObj)
	{
		hud->SetVisibleOnHUD(InObj->GetItemType(), true);
	}
}
void ARoomEscapeFPSPlayerController::ClientProcessHUDOnUpdateNextInfo_Implementation(ENextInformationType curType, ENextInformationType nextType, int32 InCount)
{
	if (ARoomEscapeFPSHUD* hud = Cast<ARoomEscapeFPSHUD>(GetHUD()))
	{
		hud->UpdateNextInfo(curType, nextType, InCount);
	}
}

// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameFramework/RoomEscapeFPSGameMode.h"
#include "GameFramework/RoomEscapeFPSHUD.h"
#include "GameFramework/RoomEscapeFPSGameState.h"
#include "GameFramework/RoomEscapeFPSPlayerController.h"
#include "Gameplay/GhostSpawner.h"
#include "Character/RoomEscapeFPSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Helper/Helper.h"

ARoomEscapeFPSGameMode::ARoomEscapeFPSGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Resources/Character/Blueprints/RoomEscapeFPSCharacter_BP"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ARoomEscapeFPSHUD::StaticClass();
}

void ARoomEscapeFPSGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void ARoomEscapeFPSGameMode::PostLogin(APlayerController* NewPlayer)
{
	ARoomEscapeFPSHUD* hud = Cast<ARoomEscapeFPSHUD>(NewPlayer->GetHUD());
	if (hud)
	{
		hud->InitializeHUD();
		hud->SetVisibilityLoadingScreen(false);
	}
}

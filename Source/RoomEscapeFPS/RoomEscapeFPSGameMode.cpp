// Copyright Epic Games, Inc. All Rights Reserved.

#include "RoomEscapeFPSGameMode.h"
#include "RoomEscapeFPSHUD.h"
#include "RoomEscapeFPSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Helper/Helper.h"
#include "UI/InteractionPanel.h"

ARoomEscapeFPSGameMode::ARoomEscapeFPSGameMode()
	: Super()
{
	//// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Resources/Blueprints/FirstPersonCharacter"));
	//DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ARoomEscapeFPSHUD::StaticClass();
}

void ARoomEscapeFPSGameMode::BeginPlay()
{
	Super::BeginPlay();
}
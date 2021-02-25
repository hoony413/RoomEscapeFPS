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

	Clue_1_Answer = 170;
	Clue_2_Answer = 132;
}

bool ARoomEscapeFPSGameMode::CheckAnswer(const int32 answer, EServerSolutionType InType)
{
	if (InType == EServerSolutionType::ESolution_1)
	{
		return answer == Clue_1_Answer;
	}
	else if (InType == EServerSolutionType::ESolution_2)
	{
		return answer == Clue_2_Answer;
	}

	return false;
}

void ARoomEscapeFPSGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	ARoomEscapeFPSPlayerController* pc = Cast<ARoomEscapeFPSPlayerController>(NewPlayer);
	if (pc)
	{
		pc->ClientSetupHUD();
	}
}


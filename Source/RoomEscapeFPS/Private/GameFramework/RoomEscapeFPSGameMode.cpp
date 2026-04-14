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
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Character/Blueprints/RoomEscapeFPSCharacter_BP"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ARoomEscapeFPSHUD::StaticClass();
}

void ARoomEscapeFPSGameMode::BeginPlay()
{
	Super::BeginPlay();

	Clue_1_Answer = 170;
	Clue_2_Answer = 132;
	GhostDeadCount_Target = 10;
}

bool ARoomEscapeFPSGameMode::CheckAnswer(const int32 answer, EServerSolutionType InType)
{
	if (InType == EServerSolutionType::SOLUTION_1)
	{
		return answer == Clue_1_Answer;
	}
	if (InType == EServerSolutionType::SOLUTION_2)
	{
		return answer == Clue_2_Answer;
	}
	if (InType == EServerSolutionType::GHOST_DEAD_COUNT_TARGET)
	{
		return answer >= GhostDeadCount_Target;
	}

	return false;
}
void ARoomEscapeFPSGameMode::SetActiveGhostSpawner(bool bActive)
{
	ARoomEscapeFPSGameState* gs = GetGameState<ARoomEscapeFPSGameState>();
	if (gs)
	{
		gs->SetActiveGhostSpawner(bActive);
	}
}
void ARoomEscapeFPSGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (ARoomEscapeFPSPlayerController* pc = Cast<ARoomEscapeFPSPlayerController>(NewPlayer))
	{
		pc->ClientSetupHUD();
	}
}


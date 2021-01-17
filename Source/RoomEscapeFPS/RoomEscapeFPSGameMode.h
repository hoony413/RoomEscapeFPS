// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RoomEscapeFPSGameMode.generated.h"

UCLASS(minimalapi)
class ARoomEscapeFPSGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ARoomEscapeFPSGameMode();

	virtual void BeginPlay() override;
};




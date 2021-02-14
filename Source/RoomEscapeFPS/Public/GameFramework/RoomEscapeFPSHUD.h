// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "RoomEscapeFPSHUD.generated.h"

UCLASS()
class ARoomEscapeFPSHUD : public AHUD
{
	GENERATED_BODY()

public:
	ARoomEscapeFPSHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;
	virtual void BeginPlay() override;

	class UInventoryPanel* GetInventoryPanel();

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

	TWeakObjectPtr<class UInventoryPanel> cachedPanel;
};


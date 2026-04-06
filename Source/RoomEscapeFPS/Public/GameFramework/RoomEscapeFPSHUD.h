// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Gameplay/TypeInfoHeader.h"
#include "RoomEscapeFPSHUD.generated.h"

class UBaseHUDWidget;

UCLASS()
class ARoomEscapeFPSHUD : public AHUD
{
	GENERATED_BODY()

public:
	/** Primary draw call for the HUD */
	void DrawHUD() override;
	void BeginPlay() override;

	void InitializeHUD();

	class UInventoryPanel* GetInventoryPanel();

	void SetVisibleOnHUD(EItemType InType, bool bOnOff);

	void UpdateNextInfo(ENextInformationType curType, ENextInformationType nextType, int32 InCount);
	UBaseHUDWidget* GetHUDWidget() const { return _hudWidget;}

protected:
	void SetVisibleBatteryInfo(bool bOnOff);
	void SetVisibleCharmInfo(bool bOnOff);
	void SetVisibleCrossHair(bool bOnOff);

private:
	UPROPERTY(Transient)
	TObjectPtr<UBaseHUDWidget> _hudWidget;
};


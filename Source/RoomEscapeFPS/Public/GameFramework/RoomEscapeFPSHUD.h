// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Gameplay/TypeInfoHeader.h"
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

	void InitializeHUD();

	void SetVisibilityLoadingScreen(bool bOpen);

	class UInventoryPanel* GetInventoryPanel();

	void SetVisibleOnHUD(EItemType InType, bool bOnOff);

	class UPipeGameUI* GetPipeGameUI();
	FORCEINLINE void CachPipeGameUI(class UPipeGameUI* InUI) { cachedPipeGameUI = InUI; }

	void UpdateNextInfo(ENextInformationType curType, ENextInformationType nextType, int32 InCount);

protected:
	void SetVisibleBatteryInfo(bool bOnOff);
	void SetVisibleCharmInfo(bool bOnOff);
	void SetVisibleCrossHair(bool bOnOff);

private:
	///** Crosshair asset pointer */
	//class UTexture2D* CrosshairTex;

	TWeakObjectPtr<class UInventoryPanel> cachedPanel;
	TWeakObjectPtr<class ULoadingScreen> cachedLoading;
	TWeakObjectPtr<class UPipeGameUI> cachedPipeGameUI;
};


// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameFramework/RoomEscapeFPSHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "UObject/ConstructorHelpers.h"
#include "Helper/Helper.h"
#include "Managers/UIManager.h"
#include "Gameplay/TypeInfoHeader.h"
#include "UI/InventoryPanel.h"
#include "Managers/UIManager.h"
#include "Helper/Helper.h"
#include "UI/LoadingScreen.h"
#include "UI/PipeGameUI.h"

ARoomEscapeFPSHUD::ARoomEscapeFPSHUD()
{
	// Set the crosshair texture
	//static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("/Game/Resources/Textures/FirstPersonCrosshair"));
	//CrosshairTex = CrosshairTexObj.Object;
}


void ARoomEscapeFPSHUD::DrawHUD()
{
	Super::DrawHUD();
}
void ARoomEscapeFPSHUD::InitializeHUD()
{
	// TODO: GameMode에서 JoinSession 완료되면 클라이언트로 RPC하도록 로직 수정
	cachedPanel = GetUIMgr()->OpenWidget<UInventoryPanel>();

	// 후레쉬, 부적 획득할 때 Visibility를 true로 설정하고, 초기값은 false.
	SetVisibleBatteryInfo(false);
	SetVisibleCharmInfo(false);
	SetVisibleCrossHair(true);
}
void ARoomEscapeFPSHUD::BeginPlay()
{
	Super::BeginPlay();
}

UInventoryPanel* ARoomEscapeFPSHUD::GetInventoryPanel()
{
	if (cachedPanel.IsValid())
	{
		return cachedPanel.Get();
	}
	return nullptr;
}
class UPipeGameUI* ARoomEscapeFPSHUD::GetPipeGameUI()
{
	if (cachedPipeGameUI.IsValid())
	{
		return cachedPipeGameUI.Get();
	}
	return nullptr;
}

void ARoomEscapeFPSHUD::SetVisibleOnHUD(EItemType InType, bool bOnOff)
{
	switch (InType)
	{
	case EItemType::Flash:
		SetVisibleBatteryInfo(bOnOff);
		break;
	case EItemType::Charm:
		SetVisibleCharmInfo(bOnOff);
		break;
	}
}
void ARoomEscapeFPSHUD::SetVisibilityLoadingScreen(bool bOpen)
{
	if (bOpen)
	{
		cachedLoading = GetUIMgr()->OpenWidget<ULoadingScreen>();
	}
	else
	{
		if (cachedLoading.IsValid())
		{
			cachedLoading->RemoveFromParent();
		}
		cachedLoading = nullptr;
	}
}
void ARoomEscapeFPSHUD::SetVisibleBatteryInfo(bool bOnOff)
{
	check(cachedPanel.IsValid());
	cachedPanel->SetFlashBatteryVisibility(bOnOff);
}
void ARoomEscapeFPSHUD::SetVisibleCharmInfo(bool bOnOff)
{
	check(cachedPanel.IsValid());
	cachedPanel->SetCharmVisibility(bOnOff);
}
void ARoomEscapeFPSHUD::SetVisibleCrossHair(bool bOnOff)
{
	check(cachedPanel.IsValid());
	cachedPanel->SetCrossHairVisibility(bOnOff);
}
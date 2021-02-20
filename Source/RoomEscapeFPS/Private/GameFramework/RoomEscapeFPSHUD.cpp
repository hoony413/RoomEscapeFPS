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
	// TODO: GameMode���� JoinSession �Ϸ�Ǹ� Ŭ���̾�Ʈ�� RPC�ϵ��� ���� ����
	cachedPanel = GetUIMgr()->OpenWidget<UInventoryPanel>();

	// �ķ���, ���� ȹ���� �� Visibility�� true�� �����ϰ�, �ʱⰪ�� false.
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
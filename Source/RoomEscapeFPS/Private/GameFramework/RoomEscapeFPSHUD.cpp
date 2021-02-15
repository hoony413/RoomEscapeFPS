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
#include "UI/BasePage.h"

ARoomEscapeFPSHUD::ARoomEscapeFPSHUD()
{
	// Set the crosshair texture
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("/Game/Resources/Textures/FirstPersonCrosshair"));
	CrosshairTex = CrosshairTexObj.Object;
}


void ARoomEscapeFPSHUD::DrawHUD()
{
	Super::DrawHUD();

	// Draw very simple crosshair

	// find center of the Canvas
	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

	// offset by half the texture's dimensions so that the center of the texture aligns with the center of the Canvas
	const FVector2D CrosshairDrawPosition( (Center.X),
										   (Center.Y + 20.0f));

	// draw the crosshair
	FCanvasTileItem TileItem( CrosshairDrawPosition, CrosshairTex->Resource, FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem( TileItem );
}

void ARoomEscapeFPSHUD::BeginPlay()
{
	Super::BeginPlay();

	cachedPanel = GetUIMgr()->OpenWidget<UInventoryPanel>();

	// 후레쉬, 부적 획득할 때 Visibility를 true로 설정하고, 초기값은 false.
	SetVisibleBatteryInfo(false);
	SetVisibleCharmInfo(false);
}

UInventoryPanel* ARoomEscapeFPSHUD::GetInventoryPanel()
{
	if (cachedPanel.IsValid())
	{
		return cachedPanel.Get();
	}
	return nullptr;
}
void ARoomEscapeFPSHUD::SetVisibleOnHUD(EItemType InType, bool bOnOff)
{
	switch (InType)
	{
	case EItemType::Flash:
		SetVisibleBatteryInfo(bOnOff);
		// TODO: 후레쉬 안내 위젯 생성
		break;
	case EItemType::Charm:
		SetVisibleCharmInfo(bOnOff);
		// TODO: 부적 안내 위젯 생성
		break;
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
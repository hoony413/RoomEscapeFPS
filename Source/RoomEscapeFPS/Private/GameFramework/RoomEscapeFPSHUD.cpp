// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameFramework/RoomEscapeFPSHUD.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "UObject/ConstructorHelpers.h"
#include "Gameplay/TypeInfoHeader.h"
#include "UI/InventoryPanel.h"
#include "Managers/UISettings.h"
#include "UI/BaseHUDWidget.h"

void ARoomEscapeFPSHUD::DrawHUD()
{
	Super::DrawHUD();
	//InitializeHUD();
}
void ARoomEscapeFPSHUD::InitializeHUD()
{
	if (_hudWidget)
	{
		return;
	}

	const UUISettings* settings = UUISettings::Get();
	if (not settings)
	{
		return;
	}
	UClass* hudClass = settings->HUDWidgetClass.LoadSynchronous();
	if (not hudClass)
	{
		return;
	}
	
	_hudWidget = CreateWidget<UBaseHUDWidget>(GetOwningPlayerController(), hudClass);
	if (_hudWidget)
	{
		_hudWidget->AddToPlayerScreen();
		if (UInventoryPanel* panel = _hudWidget->GetInventoryPanel())
		{
			panel->SetFlashBatteryVisibility(false);
			panel->SetCharmVisibility(false);
			panel->SetCrossHairVisibility(true);
			panel->UpdateNextInformation(ENextInformationType::FIND_LANTERN, ENextInformationType::FIND_LANTERN, 0);
		}
	}
}
void ARoomEscapeFPSHUD::BeginPlay()
{
	Super::BeginPlay();
}

UInventoryPanel* ARoomEscapeFPSHUD::GetInventoryPanel()
{
	return _hudWidget ? _hudWidget->GetInventoryPanel() : nullptr;
}
void ARoomEscapeFPSHUD::UpdateNextInfo(ENextInformationType curType, ENextInformationType nextType, int32 InCount)
{
	if (UInventoryPanel* panel = GetInventoryPanel())
	{
		panel->UpdateNextInformation(curType, nextType, InCount);
	}
}
void ARoomEscapeFPSHUD::SetVisibleOnHUD(EItemType InType, bool bOnOff)
{
	switch (InType)
	{
	case EItemType::FLASH:
		SetVisibleBatteryInfo(bOnOff);
		break;
	case EItemType::CHARM:
		SetVisibleCharmInfo(bOnOff);
		break;
	}
}
void ARoomEscapeFPSHUD::SetVisibleBatteryInfo(bool bOnOff)
{
	if (UInventoryPanel* panel = GetInventoryPanel())
	{
		panel->SetFlashBatteryVisibility(bOnOff);
	}
}
void ARoomEscapeFPSHUD::SetVisibleCharmInfo(bool bOnOff)
{
	if (UInventoryPanel* panel = GetInventoryPanel())
	{
		panel->SetCharmVisibility(bOnOff);
	}
}
void ARoomEscapeFPSHUD::SetVisibleCrossHair(bool bOnOff)
{
	if (UInventoryPanel* panel = GetInventoryPanel())
	{
		panel->SetCrossHairVisibility(bOnOff);
	}
}
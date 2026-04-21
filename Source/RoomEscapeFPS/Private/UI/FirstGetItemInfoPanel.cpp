// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FirstGetItemInfoPanel.h"
#include "Managers/UISubsystem.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Kismet/KismetRenderingLibrary.h"

void UFirstGetItemInfoPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (CloseButton && not CloseButton->OnClicked.IsBound())
	{
		CloseButton->OnClicked.AddDynamic(this, &UFirstGetItemInfoPanel::OnClickedCloseButton);
	}
}

void UFirstGetItemInfoPanel::SetItemNameText(FText const& InText)
{
	if (ItemNameText)
	{
		ItemNameText->SetText(InText);
	}
}
void UFirstGetItemInfoPanel::SetItemDescText(FText const& InText)
{
	if (ItemDescText)
	{
		ItemDescText->SetText(InText);
	}
}
void UFirstGetItemInfoPanel::OnClickedCloseButton()
{
	UUISubsystem* uiSubsystem = GetGameInstance()->GetSubsystem<UUISubsystem>();
	if (uiSubsystem)
	{
		uiSubsystem->CloseWidget(this);
	}
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FirstGetItemInfoPanel.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Runtime/UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/KismetRenderingLibrary.h"

void UFirstGetItemInfoPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(GetOwningPlayer(), this);
	GetOwningPlayer()->SetShowMouseCursor(true);
	if (CloseButton && CloseButton->OnClicked.IsBound() == false)
	{
		CloseButton->OnClicked.AddDynamic(this, &UFirstGetItemInfoPanel::OnClickedCloseButton);
	}
}

void UFirstGetItemInfoPanel::SetItemNameText(const FString& InStr)
{
	if (ItemNameText)
	{
		FText txt = FText::FromString(InStr);
		ItemNameText->SetText(txt);
	}
}
void UFirstGetItemInfoPanel::SetItemDescText(const FString& InStr)
{
	if (ItemDescText)
	{
		FText txt = FText::FromString(InStr);
		ItemDescText->SetText(txt);
	}
}
void UFirstGetItemInfoPanel::OnClickedCloseButton()
{
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(GetOwningPlayer());
	GetOwningPlayer()->SetShowMouseCursor(false);
	RemoveFromParent();
}
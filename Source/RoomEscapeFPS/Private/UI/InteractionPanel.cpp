// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InteractionPanel.h"
#include "Components/TextBlock.h"

void UInteractionPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UInteractionPanel::SetText(FName& str)
{
	if (InfoText != nullptr)
	{
		InfoText->SetText(FText::FromName(str));
	}
}
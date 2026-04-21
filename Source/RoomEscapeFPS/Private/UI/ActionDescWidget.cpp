// Fill out your copyright notice in the Description page of Project Settings.
#include "UI/ActionDescWidget.h"

#include "CommonActionWidget.h"
#include "CommonTextBlock.h"
#include "InputAction.h"

void UActionDescWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (not IsValid(BindInputAction))
	{
		return;
	}

	ActionWidget->SetEnhancedInputAction(BindInputAction);
	ActionTextBlock->SetText(ActionWidget->GetDisplayText());
}

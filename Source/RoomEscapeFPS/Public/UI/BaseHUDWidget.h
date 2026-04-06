// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "BaseHUDWidget.generated.h"

class UCommonActivatableWidgetStack;
class UCanvasPanel;
class UInteractionPanel;
class UInventoryPanel;

/**
 * HUD의 루트 위젯.
 * - InventoryPanel : HUD 상시 표시 요소 (배터리, 부적, 크로스헤어). BP에서 직접 소유.
 * - WidgetStack    : UCommonActivatableWidget 파생 위젯들의 풀스크린 페이지 전환용.
 * - PopupPanel     : 팝업 위젯들의 배치를 담당한다.
 */
UCLASS()
class ROOMESCAPEFPS_API UBaseHUDWidget : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	UCommonActivatableWidgetStack* GetWidgetStack() const { return WidgetStack; }
	UCanvasPanel* GetPopupPanel() const { return PopupPanel; }
	UInventoryPanel* GetInventoryPanel() const { return InventoryPanel.Get(); }
	UInteractionPanel* GetInteractionPanel() const { return InteractionPanel; }

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonActivatableWidgetStack> WidgetStack;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> PopupPanel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInventoryPanel> InventoryPanel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInteractionPanel> InteractionPanel;
};

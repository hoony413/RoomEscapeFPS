// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseWidget.h"
#include "GameFramework/RoomEscapeFPSPlayerState.h"
#include "PipeGame_Node.generated.h"

/**
 * ������ ��� UI Ŭ����
 */

UCLASS()
class ROOMESCAPEFPS_API UPipeGame_Node : public UBaseWidget
{
	GENERATED_BODY()
	
public:
	void InitializePipeNode(FPipeNode& InNode);

	virtual FString GetBPPath() { return TEXT("WidgetBlueprint'/Game/Resources/Widgets/PipeGame_Node_Widget.PipeGame_Node_Widget_C'"); }

protected:
	UFUNCTION()
		void OnClickedPipeButton();

private:
	// ���� ��ư
	UPROPERTY(meta = (BindWidget))
		class UButton* PipeButton;
	UPROPERTY(meta = (BindWidget))
		class UImage* PipeImage;

	UPROPERTY()
	FPipeNode PipeNodeRef;

	uint8 RotationInfo = 0u;

	UPROPERTY(EditDefaultsOnly, Category = "Sprite Assets")
		TSoftObjectPtr<class UPaperSprite> Straight_Two;
	UPROPERTY(EditDefaultsOnly, Category = "Sprite Assets")
		TSoftObjectPtr<class UPaperSprite> Two;
	UPROPERTY(EditDefaultsOnly, Category = "Sprite Assets")
		TSoftObjectPtr<class UPaperSprite> Three;
	UPROPERTY(EditDefaultsOnly, Category = "Sprite Assets")
		TSoftObjectPtr<class UPaperSprite> Four;
};

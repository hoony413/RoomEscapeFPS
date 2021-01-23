// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseWidget.h"
#include "GameFramework/RoomEscapeFPSPlayerState.h"
#include "PipeGame_Node.generated.h"

/**
 * 파이프 노드 UI 클래스
 */

UCLASS(Config=Game)
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
	// 위젯 버튼
	UPROPERTY(meta = (BindWidget))
		class UButton* PipeButton;
	UPROPERTY(meta = (BindWidget))
		class UImage* PipeImage;

	UPROPERTY()
	FPipeNode PipeNodeRef;

	uint8 RotationInfo = 0u;

	UPROPERTY(Config)
		TArray<FSoftObjectPath> UITextureAssets;

	UPROPERTY(EditDefaultsOnly, Category = "Texture Assets")
		TSoftObjectPtr<UTexture2D> Straight_Two;
	UPROPERTY(EditDefaultsOnly, Category = "Texture Assets")
		TSoftObjectPtr<UTexture2D> Two;
	UPROPERTY(EditDefaultsOnly, Category = "Texture Assets")
		TSoftObjectPtr<UTexture2D> Three;
	UPROPERTY(EditDefaultsOnly, Category = "Texture Assets")
		TSoftObjectPtr<UTexture2D> Four;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Base/BaseWidget.h"
#include "GameFramework/RoomEscapeFPSPlayerState.h"
#include "PipeGame_Node.generated.h"

/**
 * 파이프 노드 UI 클래스
 */

class UPaperSprite;
class UButton;
class UImage;

UENUM()
enum class EAnimationIndex
{
	NONE = 0 UMETA(Hidden),
	ROTATE_90,
	ROTATE_180,
	ROTATE_270,
	ROTATE_360,
	RESULT,
	MAX UMETA(Hidden)
};

UCLASS()
class ROOMESCAPEFPS_API UPipeGame_Node : public UBaseWidget
{
	GENERATED_BODY()
	
public:
	void InitializePipeNode(FPipeNode& InNode, uint8 InGridSize);

	void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;

	void PlayResultAnimation();
	FPipeNode& GetPipeNodeRef() { return PipeNodeRef; }

protected:
	UFUNCTION()
		void OnClickedPipeButton();

private:
	// 위젯 버튼
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> PipeButton;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> PipeImage;

	UPROPERTY()
	FPipeNode PipeNodeRef;

	uint8 RotationInfo = 0u;
	uint8 GridSize = 0u;

	UPROPERTY(EditDefaultsOnly, Category = "Sprite Assets")
	TSoftObjectPtr<UPaperSprite> Straight_Two;

	UPROPERTY(EditDefaultsOnly, Category = "Sprite Assets")
	TSoftObjectPtr<UPaperSprite> Two;

	UPROPERTY(EditDefaultsOnly, Category = "Sprite Assets")
	TSoftObjectPtr<UPaperSprite> Three;

	UPROPERTY(EditDefaultsOnly, Category = "Sprite Assets")
	TSoftObjectPtr<UPaperSprite> Four;
};

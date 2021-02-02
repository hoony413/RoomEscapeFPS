// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseWidget.h"
#include "GameFramework/RoomEscapeFPSPlayerState.h"
#include "UI/PipeGameUI.h"
#include "PipeGame_Node.generated.h"

/**
 * 파이프 노드 UI 클래스
 */

UENUM()
enum class EAnimationIndex
{
	ERotate_90,
	ERotate_180,
	ERotate_270,
	ERotate_360,
	EResult,
};

UCLASS()
class ROOMESCAPEFPS_API UPipeGame_Node : public UBaseWidget
{
	GENERATED_BODY()
	
public:
	void InitializePipeNode(FPipeNode& InNode, uint8 InGridSize);

	virtual FString GetBPPath() { return TEXT("WidgetBlueprint'/Game/Resources/Widgets/PipeGame_Node_Widget.PipeGame_Node_Widget_C'"); }
	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;

	void PlayResultAnimation();

	FPipeNode& GetPipeNodeRef() { return PipeNodeRef; }

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
	uint8 GridSize = 0u;


	UPROPERTY(EditDefaultsOnly, Category = "Sprite Assets")
		TSoftObjectPtr<class UPaperSprite> Straight_Two;
	UPROPERTY(EditDefaultsOnly, Category = "Sprite Assets")
		TSoftObjectPtr<class UPaperSprite> Two;
	UPROPERTY(EditDefaultsOnly, Category = "Sprite Assets")
		TSoftObjectPtr<class UPaperSprite> Three;
	UPROPERTY(EditDefaultsOnly, Category = "Sprite Assets")
		TSoftObjectPtr<class UPaperSprite> Four;
};

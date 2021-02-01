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
enum class EResultAnimType
{
	EUpToRight = 0x01,
	EUpToDown = 0x02,
	ELeftToRight = 0x04,
	ELeftToDown = 0x08,
};

UENUM()
enum class EAnimNum
{
	EUtoC = 4,
	ELtoC = 5,
	ECtoD = 6,
	ECtoR = 7,
};

UCLASS()
class ROOMESCAPEFPS_API UPipeGame_Node : public UBaseWidget
{
	GENERATED_BODY()
	
public:
	void InitializePipeNode(FPipeNode& InNode, uint8 InGridSize, FAnswerNodeAnimNotiDelegate InDelegate);

	virtual FString GetBPPath() { return TEXT("WidgetBlueprint'/Game/Resources/Widgets/PipeGame_Node_Widget.PipeGame_Node_Widget_C'"); }
	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;

	void PlayResultAnimation(EResultAnimType InAnimType);

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

	EResultAnimType cachedAnimType;

	FAnswerNodeAnimNotiDelegate AnimDelegate;

	UPROPERTY(EditDefaultsOnly, Category = "Sprite Assets")
		TSoftObjectPtr<class UPaperSprite> Straight_Two;
	UPROPERTY(EditDefaultsOnly, Category = "Sprite Assets")
		TSoftObjectPtr<class UPaperSprite> Two;
	UPROPERTY(EditDefaultsOnly, Category = "Sprite Assets")
		TSoftObjectPtr<class UPaperSprite> Three;
	UPROPERTY(EditDefaultsOnly, Category = "Sprite Assets")
		TSoftObjectPtr<class UPaperSprite> Four;
};

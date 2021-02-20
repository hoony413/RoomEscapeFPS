// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseWidget.h"
#include "PipeGameUI.generated.h"

/**
 * 
 */

UCLASS()
class ROOMESCAPEFPS_API UPipeGameUI : public UBaseWidget
{
	GENERATED_BODY()
	
public:
	void InitializeGrid(TArray<struct FPipeNode>& PipeNodesInfo, uint8 InGridSize);

	virtual FString GetBPPath() { return TEXT("WidgetBlueprint'/Game/Resources/Widgets/InGame/PipeGame_Widget.PipeGame_Widget_C'"); }

	void CheckCommittedAnswerAnimation(bool bSuccess);

	void CloseUI();

protected:

	UFUNCTION()
		void OnClickedCommitButton();
	UFUNCTION()
		void OnClickedCloseButton();
	UFUNCTION()
		void LaunchFlowAnimation(int32 gridIndex, bool bLastNode);

private:
	UPROPERTY(meta = (BindWidget))
		class UUniformGridPanel* PipeGrid;
	UPROPERTY(meta = (BindWidget))
		class UButton* CloseButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* CommitButton;

	bool bRequested = false;
	bool bSucceed = false;

	int32 GridSize;

	UPROPERTY(EditDefaultsOnly, Category = "PipeNode Widget")
		TSoftClassPtr<class UPipeGame_Node> NodeWidget;
};

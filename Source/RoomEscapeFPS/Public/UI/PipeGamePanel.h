// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Base/BaseActivatableWidget.h"
#include "PipeGamePanel.generated.h"

struct FPipeNode;
class UUniformGridPanel;
class UButton;
class UPipeGame_Node;

/**
 *
 */

UCLASS()
class ROOMESCAPEFPS_API UPipeGamePanel : public UBaseActivatableWidget
{
	GENERATED_BODY()

public:
	void NativeOnInitialized() override;
	void InitializeGrid(TArray<FPipeNode> const& PipeNodesInfo, uint8 InGridSize);
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
	TObjectPtr<UUniformGridPanel> PipeGrid;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CloseButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CommitButton;

	bool bRequested = false;
	bool bSucceed = false;

	int32 GridSize;

	UPROPERTY(EditDefaultsOnly, Category = "PipeNode Widget")
	TSubclassOf<UPipeGame_Node> NodeWidget;
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PipeGameUI.h"
#include "Helper/Helper.h"
#include "Managers/UIManager.h"
#include "Components/Button.h"
#include "Components/UniformGridPanel.h"
#include "Gameplay/PipeGameInfo.h"
#include "Runtime/UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "UI/PipeGame_Node.h"

void UPipeGameUI::InitializeGrid(TArray<FPipeNode>& PipeNodesInfo, uint8 InGridSize)
{
	UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(GetOwningPlayer(), this);
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
	CommitButton->OnClicked.AddDynamic(this, &UPipeGameUI::OnClickedCommitButton);
	CloseButton->OnClicked.AddDynamic(this, &UPipeGameUI::OnClickedCloseButton);

	GridSize = InGridSize;
	AnimDelegate.BindUObject(this, &UPipeGameUI::LaunchNextAnim);

	check(PipeGrid);
	int32 rowCol = 0;
	for (auto& elem : PipeNodesInfo)
	{
		UPipeGame_Node* nodeWidget = GetUIMgr()->GetWidget<UPipeGame_Node>();
		
		if (nodeWidget)
		{
			PipeGrid->AddChildToUniformGrid(nodeWidget, rowCol / InGridSize, rowCol % InGridSize);
			nodeWidget->InitializePipeNode(elem, InGridSize, AnimDelegate);
			++rowCol;
		}
	}
}
void UPipeGameUI::CheckCommittedAnswerAnimation(bool bSuccess)
{
	// TODO: ��� ���� ����(����/����).
	LaunchNextAnim(0);
}
void UPipeGameUI::OnClickedCommitButton()
{
	// TODO: ��������Ʈ�� �ݺ��ؼ� ��û�� ���� �� �ִ�. ���� �� �ִ� ��� ���.
	if (!bRequested)
		bRequested = true;
	else
		return;

	// ����� ������ ������ �����ؾ��ϹǷ� ���� �˾��� ĳ���Ѵ�.
	GetUIMgr()->CachPipeGameUI(this);
	ARoomEscapeFPSPlayerState* ps = GetOwningPlayerState<ARoomEscapeFPSPlayerState>(true);
	if (ps)
	{
		ps->ServerCheckCommittedAnswer();
	}
}
void UPipeGameUI::OnClickedCloseButton()
{
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(GetOwningPlayer());
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
	bRequested = false;

	ARoomEscapeFPSPlayerState* ps = GetOwningPlayerState<ARoomEscapeFPSPlayerState>(true);
	if (ps)
	{
		ps->ServerClearPipeGame();
	}

	RemoveFromParent();
}

void UPipeGameUI::LaunchNextAnim(int32 Index)
{
	if (Index >= (GridSize * GridSize))
	{
		// TODO: ������ ������. ����/���� �ǵ�� �� ���� �˾��� �ݱ�.
		OnClickedCloseButton();
		return;
	}
	ARoomEscapeFPSPlayerState* ps = GetOwningPlayerState<ARoomEscapeFPSPlayerState>(true);
	if (ps)
	{
		FPipeGameInfo& pipeInfo = ps->GetPipeGameInfo();
		UPipeGame_Node* nodeWidget = Cast<UPipeGame_Node>(PipeGrid->GetChildAt(Index));
		if (nodeWidget)
		{
			FPipeNode& nodeRef = nodeWidget->GetPipeNodeRef();
			if (pipeInfo.IsConnected(nodeRef, EPipeDirection::EUp))
			{
				if (pipeInfo.IsConnected(nodeRef, EPipeDirection::EDown))
				{
					nodeWidget->PlayResultAnimation(EResultAnimType::EUpToDown);
				}
				else if (pipeInfo.IsConnected(nodeRef, EPipeDirection::ERight))
				{
					nodeWidget->PlayResultAnimation(EResultAnimType::EUpToRight);
				}
			}
			else if (pipeInfo.IsConnected(nodeRef, EPipeDirection::ELeft))
			{
				if (pipeInfo.IsConnected(nodeRef, EPipeDirection::EDown))
				{
					nodeWidget->PlayResultAnimation(EResultAnimType::ELeftToDown);
				}
				else if (pipeInfo.IsConnected(nodeRef, EPipeDirection::ERight))
				{
					nodeWidget->PlayResultAnimation(EResultAnimType::ELeftToRight);
				}
			}
		}
	}
}
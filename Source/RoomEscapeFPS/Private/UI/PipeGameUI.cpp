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
	// TODO: 결과 연출 시작(성공/실패).
	LaunchNextAnim(0);
}
void UPipeGameUI::OnClickedCommitButton()
{
	// TODO: 데디케이트에 반복해서 요청을 보낼 수 있다. 막을 수 있는 방법 고민.
	if (!bRequested)
		bRequested = true;
	else
		return;

	// 결과를 받으면 연출을 시작해야하므로 현재 팝업을 캐싱한다.
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
		// TODO: 연출이 끝났다. 성공/실패 피드백 후 현재 팝업은 닫기.
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
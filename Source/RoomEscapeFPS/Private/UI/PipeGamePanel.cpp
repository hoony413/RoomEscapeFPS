// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PipeGamePanel.h"
#include "Helper/Helper.h"
#include "Managers/UISubsystem.h"
#include "Components/Button.h"
#include "Components/UniformGridPanel.h"
#include "Gameplay/PipeGameInfo.h"
#include "GameFramework/RoomEscapeFPSPlayerController.h"
#include "UI/NoticePanel.h"
#include "UI/PipeGame_Node.h"

void UPipeGamePanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (CommitButton && not CommitButton->OnClicked.IsBound())
	{
		CommitButton->OnClicked.AddDynamic(this, &UPipeGamePanel::OnClickedCommitButton);
	}
	if (CloseButton && not CloseButton->OnClicked.IsBound())
	{
		CloseButton->OnClicked.AddDynamic(this, &UPipeGamePanel::OnClickedCloseButton);
	}
}

void UPipeGamePanel::InitializeGrid(TArray<FPipeNode>& PipeNodesInfo, uint8 InGridSize)
{
	GridSize = InGridSize;

	check(PipeGrid);
	UUISubsystem* uiSubsystem = Helper::GetSubsystem<UUISubsystem>(GetWorld());
	if (not uiSubsystem)
	{
		return;
	}
	int32 rowCol = 0;
	for (auto& elem : PipeNodesInfo)
	{
		if (UPipeGame_Node* nodeWidget = CreateWidget<UPipeGame_Node>(this, NodeWidget.Get()))
		{
			PipeGrid->AddChildToUniformGrid(nodeWidget, rowCol / InGridSize, rowCol % InGridSize);
			nodeWidget->InitializePipeNode(elem, InGridSize);
			++rowCol;
		}
	}
}
void UPipeGamePanel::CheckCommittedAnswerAnimation(bool bSuccess)
{
	bSucceed = bSuccess;
	if (ARoomEscapeFPSPlayerState* ps = GetOwningPlayerState<ARoomEscapeFPSPlayerState>(true))
	{
		FPipeGameInfo& pipeInfo = ps->GetPipeGameInfo();
		TArray<FPipeNode>& nodes = pipeInfo.GetPipeNodes();

		int32 j = 0;
		for(int32 i = 0; i < nodes.Num(); ++i)
		{
			if (nodes[i].IsAnswerNode())
			{
				FTimerHandle AnimationTimer;
				FTimerDelegate AnimationDele;
				int32 index = nodes[i].GetPipeLocation().X + (nodes[i].GetPipeLocation().Y * GridSize);
				AnimationDele.BindUObject(this, &UPipeGamePanel::LaunchFlowAnimation, index, nodes[i].IsLastAnswerNode());
				GetWorld()->GetTimerManager().SetTimer(AnimationTimer, AnimationDele, 0.2f, false, 0.2f * j);

				nodes[i].SetAnswerNode(false);
				j++;
			}
		}

		if (j <= 0)
		{	// 시작 노드부터 오답이다. 음수로 처리.
			LaunchFlowAnimation(-1, true);
		}
	}
}

void UPipeGamePanel::LaunchFlowAnimation(int32 gridIndex, bool bLastNode)
{
	if (gridIndex >= 0)
	{
		if (UPipeGame_Node* nodeWidget = Cast<UPipeGame_Node>(PipeGrid->GetChildAt(gridIndex)))
		{
			nodeWidget->PlayResultAnimation();
		}
	}

	// 마지막 연출인지 확인.
	if (bLastNode)
	{	// 성공인 경우
		bRequested = false;
		// 성공/실패 결과팝업 분기.
		if (UUISubsystem* uiSubsystem = Helper::GetSubsystem<UUISubsystem>(GetWorld()))
		{
			if (UNoticePanel* panel = uiSubsystem->OpenWidget<UNoticePanel>(EActivatableWidgetType::NOTICE))
			{
				panel->OpenNotice(bSucceed ? ENoticeType::SUCCESS : ENoticeType::FAILED);
			}
		}

		FTimerHandle ClosePopupTimer;
		GetWorld()->GetTimerManager().SetTimer(ClosePopupTimer, this, &UPipeGamePanel::OnClickedCloseButton,
			0.1f, false, 1.f);
	}
}

void UPipeGamePanel::OnClickedCommitButton()
{
	if (not bRequested)
	{
		bRequested = true;
		if (ARoomEscapeFPSPlayerController* pc = GetOwningPlayer<ARoomEscapeFPSPlayerController>())
		{
			pc->ServerCheckCommittedAnswer();
		}
	}
}
void UPipeGamePanel::OnClickedCloseButton()
{
	if (ARoomEscapeFPSPlayerController* pc = GetOwningPlayer<ARoomEscapeFPSPlayerController>())
	{
		pc->ServerClearPipeGame();
	}
	CloseUI();
}

void UPipeGamePanel::CloseUI()
{
	bRequested = false;

	if (UUISubsystem* uiSubsystem = Helper::GetSubsystem<UUISubsystem>(GetWorld()))
	{
		uiSubsystem->CloseWidget(this);
	}
}

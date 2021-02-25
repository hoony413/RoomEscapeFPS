// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PipeGameUI.h"
#include "Helper/Helper.h"
#include "Managers/UIManager.h"
#include "Components/Button.h"
#include "Components/UniformGridPanel.h"
#include "Gameplay/PipeGameInfo.h"
#include "Runtime/UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "GameFramework/RoomEscapeFPSHUD.h"
#include "UI/NoticePanel.h"
#include "UI/PipeGame_Node.h"

void UPipeGameUI::InitializeGrid(TArray<FPipeNode>& PipeNodesInfo, uint8 InGridSize)
{
	UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(GetOwningPlayer(), this);
	GetOwningPlayer()->SetShowMouseCursor(true);
	CommitButton->OnClicked.AddDynamic(this, &UPipeGameUI::OnClickedCommitButton);
	CloseButton->OnClicked.AddDynamic(this, &UPipeGameUI::OnClickedCloseButton);

	GridSize = InGridSize;

	check(PipeGrid);
	int32 rowCol = 0;
	for (auto& elem : PipeNodesInfo)
	{
		UPipeGame_Node* nodeWidget = GetUIMgr()->GetWidget<UPipeGame_Node>();
		
		if (nodeWidget)
		{
			PipeGrid->AddChildToUniformGrid(nodeWidget, rowCol / InGridSize, rowCol % InGridSize);
			nodeWidget->InitializePipeNode(elem, InGridSize);
			++rowCol;
		}
	}
}
void UPipeGameUI::CheckCommittedAnswerAnimation(bool bSuccess)
{
	// TODO: ��� ���� ����(����/����).
	bSucceed = bSuccess;
	ARoomEscapeFPSPlayerState* ps = GetOwningPlayerState<ARoomEscapeFPSPlayerState>(true);
	if (ps)
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
				AnimationDele.BindUObject(this, &UPipeGameUI::LaunchFlowAnimation, index, nodes[i].IsLastAnswerNode());
				GetWorld()->GetTimerManager().SetTimer(AnimationTimer, AnimationDele, 0.2f, false, 0.2f * j);
				
				nodes[i].SetAnswerNode(false);
				j++;
			}
		}

		if (j <= 0)
		{	// ���� ������ �����̴�. ������ ó��.
			LaunchFlowAnimation(-1, true);
		}
	}
}

void UPipeGameUI::LaunchFlowAnimation(int32 gridIndex, bool bLastNode)
{
	if (gridIndex >= 0)
	{
		UPipeGame_Node* nodeWidget = Cast<UPipeGame_Node>(PipeGrid->GetChildAt(gridIndex));
		if (nodeWidget)
		{
			nodeWidget->PlayResultAnimation();
		}
	}
	
	// ������ �������� Ȯ��.
	if (bLastNode)
	{	// ������ ���
		bRequested = false;
		// ����/���� ����˾� �б�.
		UNoticePanel* panel = GetUIMgr()->GetWidget<UNoticePanel>();
		if (panel)
		{
			panel->OpenNotice(bSucceed ? ENoticeType::ESuccess : ENoticeType::EFailed);
		}

		FTimerHandle ClosePopupTimer;
		GetWorld()->GetTimerManager().SetTimer(ClosePopupTimer, this, &UPipeGameUI::OnClickedCloseButton,
			0.1f, false, 1.f);
	}
}

void UPipeGameUI::OnClickedCommitButton()
{
	// TODO: ��������Ʈ�� �ݺ��ؼ� ��û�� ���� �� �ִ�. ���� �� �ִ� ��� ���.
	if (!bRequested)
	{
		bRequested = true;
		// ����� ������ ������ �����ؾ��ϹǷ� ���� �˾��� ĳ���Ѵ�.
		ARoomEscapeFPSHUD* hud = Cast<ARoomEscapeFPSHUD>(GetOwningPlayer()->GetHUD());
		if (hud)
		{
			hud->CachPipeGameUI(this);
		}
		ARoomEscapeFPSPlayerState* ps = GetOwningPlayerState<ARoomEscapeFPSPlayerState>(true);
		if (ps)
		{
			ps->ServerCheckCommittedAnswer();
		}
	}
}
void UPipeGameUI::OnClickedCloseButton()
{
	ARoomEscapeFPSPlayerState* ps = GetOwningPlayerState<ARoomEscapeFPSPlayerState>(true);
	if (ps)
	{
		ps->ServerClearPipeGame();
	}
	CloseUI();
}

void UPipeGameUI::CloseUI()
{
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(GetOwningPlayer());
	GetOwningPlayer()->SetShowMouseCursor(false);
	
	ARoomEscapeFPSHUD* hud = Cast<ARoomEscapeFPSHUD>(GetOwningPlayer()->GetHUD());
	if (hud)
	{
		hud->CachPipeGameUI(nullptr);
	}
	bRequested = false;
	RemoveFromParent();
}


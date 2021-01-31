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

	check(PipeGrid);
	int32 rowCol = 0;
	for (auto& elem : PipeNodesInfo)
	{
		UPipeGame_Node* nodeWidget = GetUIMgr()->GetWidget<UPipeGame_Node>();
		
		if (nodeWidget)
		{
			PipeGrid->AddChildToUniformGrid(nodeWidget, rowCol / InGridSize, rowCol % InGridSize);
			nodeWidget->InitializePipeNode(elem);
			++rowCol;
		}
	}
}
void UPipeGameUI::CheckCommittedAnswerAnimation(bool bSuccess)
{
	// TODO: 결과 연출 시작(성공/실패).

}
void UPipeGameUI::OnClickedCommitButton()
{
	// TODO: 데디케이트에 반복해서 요청을 보낼 수 있다. 막을 수 있는 방법 고민.
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(GetOwningPlayer());
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
	
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
	RemoveFromParent();
}
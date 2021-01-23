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
	UWidgetBlueprintLibrary::SetInputMode_UIOnly(GetOwningPlayer(), this);
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
void UPipeGameUI::OnClickedCommitButton()
{
	// TODO: �ùķ��̼� �б�. ������ �ƴ� �� ������ ȿ���� ���� ǥ��.
	// TODO: ������ ��� ������ ���� ����.
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(GetOwningPlayer());
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
}
void UPipeGameUI::OnClickedCloseButton()
{
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(GetOwningPlayer());
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
	RemoveFromParent();
}
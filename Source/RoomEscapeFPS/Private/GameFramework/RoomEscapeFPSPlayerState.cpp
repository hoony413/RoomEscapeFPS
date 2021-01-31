// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/RoomEscapeFPSPlayerState.h"
#include "GameFramework/RoomEscapeFPSGameState.h"
#include "Gameplay/PipeGameInfo.h"
#include "Helper/Helper.h"
#include "Managers/UIManager.h"
#include "UI/PipeGameUI.h"
#include "Net/UnrealNetwork.h"

//bool ARoomEscapeFPSPlayerState::ReplicateSubobjects(UActorChannel *Channel, FOutBunch *Bunch, FReplicationFlags *RepFlags)
//{
//	bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
//
//	// Single Object
//	bWroteSomething |= Channel->ReplicateSubobject(PipeGameInfo, *Bunch, *RepFlags);
//	// Array of Objects
//	bWroteSomething |= Channel->ReplicateSubobjectList(ArrayOfMyObject, *Bunch, *RepFlags);
//
//	return bWroteSomething;
//}

void ARoomEscapeFPSPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ARoomEscapeFPSPlayerState, PipeGameInfo);
}

/*
������ ����:
2x2 ~ 5x5���� �ִ� ť�� ������ ���� ��������,
�� ���(������ Ÿ��)�� ������ ���� ���� �帣�� �Ѵ�.
���� ���ʿ��� ������, ������ �Ʒ��θ� �帣��, �� Ÿ�� ���� ������ �Ʒ��� ����.

1. ���� ��� ���� ����
2. ���� ����� Ÿ�� ���� ȸ�� + ���� �߰�
3. 3������ ������ ���� ���鿡 ������ ȸ���� �ο�.
4. Ŭ���̾�Ʈ�� ������ ����.
*/

void ARoomEscapeFPSPlayerState::InitializePipeGame(uint8 InGridSize)
{
	// TODO: ��û�� �÷��̾��� GameState���� ó���ϵ��� ���� ����(���� �Լ��� ��� �̰�)
	PipeGameInfo.InitializeNodes(InGridSize);
	PipeGameInfo.SetGridSize(InGridSize);

	TArray<FPipeNode>& nodeInfo = PipeGameInfo.GetPipeNodes();
	// Ŭ���̾�Ʈ�� ��򰡿��� Server_Replicated�� ȣ��Ǿ� ���������� ���� ����.
	// �� + �� - 1 ������ ���� ��带 �����Ѵ�.
	int32 NeedNodeCount = (InGridSize * 2) - 1;
	int32 TotalNodeCount = InGridSize * InGridSize;
	FIntPoint currentPos(0, 0);

	for (int32 i = 0;;)
	{
		PipeGameInfo.AddAnswerNodeInfo(nodeInfo[i]);
		if (currentPos.X >= InGridSize - 1 && currentPos.Y >= InGridSize - 1)
		{	// Goal��忡 �����ߴ��� üũ
			break;
		}

		bool IsRight = FMath::RandBool();
		if ((currentPos.X >= InGridSize - 1 && IsRight) ||
			(currentPos.Y >= InGridSize - 1 && !IsRight))
		{
			IsRight = !IsRight;
		}

		// Output ���� �� �� ��������� Input���� ����.
		if (IsRight)
		{
			nodeInfo[i].AddDirection(EPipeDirection::ERight);
			currentPos.X += 1;
			i += 1;
			nodeInfo[i].AddDirection(EPipeDirection::ELeft);
		}
		else
		{
			nodeInfo[i].AddDirection(EPipeDirection::EDown);
			currentPos.Y += 1;
			i += InGridSize;
			nodeInfo[i].AddDirection(EPipeDirection::EUp);
		}
	}
	// ���� ����� Input�� �� ����� Output�� ���� �߰��Ѵ�.
	nodeInfo[0].AddDirection(EPipeDirection::ELeft);
	nodeInfo[TotalNodeCount - 1].AddDirection(EPipeDirection::ERight);

	// ���� ������ �Ϸ�Ǿ����� �̵��� �ڼ���, ������ Ÿ���� ���� �����Ѵ�.
	int32 nodePos = 0;
	for (auto& elem : nodeInfo)
	{
		currentPos.X = nodePos % 5;
		currentPos.Y = nodePos / 5;
		elem.SetPipeLocation(currentPos);

		// ���� ��� ������ ȸ����Ű�� + ���� �߰��ϱ�(Ȯ����)
		if (elem.IsAnswerNode())
		{
			// 0 ~ 2���� ������ �߰�
			elem.SetRandomDir(FMath::RandRange((int32)0, (int32)2));
			
			// ������ ȸ��
			int32 count = FMath::RandRange((int32)1, (int32)3);
			for (int32 i = 0; i < count; ++i)
			{
				elem.RotatePipe();
			}
		}
		else
		{
			// 2����(���� ����) 90% / 3���� 9% / 4���� 1%
			float f = FMath::RandRange(0.f, 1.f);
			if (f < 0.9f)
			{
				elem.SetRandomDir(2);
			}
			else if (f < 0.99f)
			{
				elem.SetRandomDir(3);
			}
			else
			{
				elem.SetRandomDir(4);
			}
		}

		elem.SetPipeType();
		++nodePos;
	}
}
void ARoomEscapeFPSPlayerState::OnRep_PipeGameInfo()
{
	APawn* pawn = GetPawn();
	if (pawn && pawn->IsLocallyControlled() && GetNetMode() == NM_Client)
	{
		UPipeGameUI* pipe = GetUIMgr()->GetWidget<UPipeGameUI>();
		if (pipe)
		{
			pipe->InitializeGrid(PipeGameInfo.GetPipeNodes(), PipeGameInfo.GetGridSize());
			pipe->AddToViewport();
		}
	}
}
void ARoomEscapeFPSPlayerState::ServerCheckCommittedAnswer_Implementation()
{
	if (GetNetMode() == NM_DedicatedServer)
	{
		bool bSuccess = CheckPipeAnswer();
		// TODO: Client RPC�� �ش� ������ UI���� ���� ����.
	}
}

bool ARoomEscapeFPSPlayerState::CheckPipeAnswer()
{
	TArray<FPipeNode>& nodes = PipeGameInfo.GetPipeNodes();
	int32 currentPosIndex = 0;
	int32 endPosIndex = (PipeGameInfo.GetGridSize() * PipeGameInfo.GetGridSize()) - 1;

	TArray<FPipeNode*> answerNodes;
	answerNodes.Push(&nodes[currentPosIndex]);
	while (currentPosIndex < endPosIndex)
	{
		if (nodes[currentPosIndex].IsContainDirection(EPipeDirection::ERight))
		{
			currentPosIndex += 1;
			answerNodes.Push(&nodes[currentPosIndex]);
		}
		if (nodes[currentPosIndex].IsContainDirection(EPipeDirection::EDown))
		{
			currentPosIndex += PipeGameInfo.GetGridSize();
			answerNodes.Push(&nodes[currentPosIndex]);
		}
	}

	if (answerNodes.Top()->GetPipeLocation() == nodes[endPosIndex].GetPipeLocation())
		return true;
	
	return false;
}

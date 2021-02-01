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
	DOREPLIFETIME(ARoomEscapeFPSPlayerState, bPipeGameOpened);
}

/*
파이프 게임:
2x2 ~ 5x5까지 있는 큐브 형태의 퍼즐 게임으로,
각 노드(파이프 타일)의 방향을 돌려 물을 흐르게 한다.
물은 왼쪽에서 오른쪽, 위에서 아래로만 흐르며, 맵 타일 생성 로직은 아래와 같다.

1. 정답 경로 랜덤 생성
2. 정답 경로의 타일 랜덤 회전 + 방향 추가
3. 3번에서 추출한 정답 노드들에 랜덤한 회전값 부여.
4. 클라이언트로 데이터 전송.
*/

void ARoomEscapeFPSPlayerState::InitializePipeGame(uint8 InGridSize)
{
	// TODO: 요청한 플레이어의 GameState에서 처리하도록 로직 변경(관련 함수도 모두 이관)
	PipeGameInfo.InitializeNodes(InGridSize);
	PipeGameInfo.SetGridSize(InGridSize);

	TArray<FPipeNode>& nodeInfo = PipeGameInfo.GetPipeNodes();
	// 클라이언트의 어딘가에서 Server_Replicated로 호출되어 파이프게임 생성 시작.
	// 행 + 열 - 1 개수의 정답 노드를 생성한다.
	int32 NeedNodeCount = (InGridSize * 2) - 1;
	int32 TotalNodeCount = InGridSize * InGridSize;
	FIntPoint currentPos(0, 0);

	for (int32 i = 0;;)
	{
		nodeInfo[i].SetAnswerNode(true);
		if (currentPos.X >= InGridSize - 1 && currentPos.Y >= InGridSize - 1)
		{	// Goal노드에 도착했는지 체크
			break;
		}

		bool IsRight = FMath::RandBool();
		if ((currentPos.X >= InGridSize - 1 && IsRight) ||
			(currentPos.Y >= InGridSize - 1 && !IsRight))
		{
			IsRight = !IsRight;
		}

		// Output 설정 후 그 다음노드의 Input으로 연결.
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
	// 시작 노드의 Input과 골 노드의 Output은 직접 추가한다.
	nodeInfo[0].AddDirection(EPipeDirection::ELeft);
	nodeInfo[TotalNodeCount - 1].AddDirection(EPipeDirection::ERight);

	// 정답 생성이 완료되었으면 이들을 뒤섞고, 나머지 타일을 랜덤 생성한다.
	int32 nodePos = 0;
	for (auto& elem : nodeInfo)
	{
		currentPos.X = nodePos % 5;
		currentPos.Y = nodePos / 5;
		elem.SetPipeLocation(currentPos);

		// 정답 노드 무작위 회전시키기 + 방향 추가하기(확률성)
		if (elem.IsAnswerNode())
		{
			// 0 ~ 2개의 방향을 추가
			if (FMath::RandBool())
			{
				elem.SetRandomDir(FMath::RandRange((int32)0, (int32)2));
			}
			
			// 파이프 회전
			int32 count = FMath::RandRange((int32)1, (int32)3);
			for (int32 i = 0; i < count; ++i)
			{
				elem.RotatePipe();
			}
		}
		else
		{
			// 2방향(직선 포함) 90% / 3방향 9% / 4방향 1%
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

		// 정답 플래그 초기화
		elem.SetAnswerNode(false);
		elem.SetPipeType();
		++nodePos;
	}
}
void ARoomEscapeFPSPlayerState::OnRep_PipeGameInfo()
{
	if (!bPipeGameOpened)
	{
		if (GetNetMode() == NM_Client)
		{
			APawn* pawn = GetPawn();
			if (pawn && pawn->IsLocallyControlled() && GetNetMode() == NM_Client)
			{
				UPipeGameUI* pipe = GetUIMgr()->GetWidget<UPipeGameUI>();
				if (pipe)
				{
					pipe->InitializeGrid(PipeGameInfo.GetPipeNodes(), PipeGameInfo.GetGridSize());
					pipe->AddToPlayerScreen();
				}
			}
		}

		bPipeGameOpened = true;
	}
}
void ARoomEscapeFPSPlayerState::ServerRotatePipe_Implementation(int32 Index)
{
	if (GetNetMode() == NM_DedicatedServer)
	{
		TArray<FPipeNode>& nodeInfo = PipeGameInfo.GetPipeNodes();
		check(nodeInfo.Num() > Index);
		nodeInfo[Index].RotatePipe();
	}
}
void ARoomEscapeFPSPlayerState::ServerCheckCommittedAnswer_Implementation()
{
	if (GetNetMode() == NM_DedicatedServer)
	{
		bool bSuccess = CheckPipeAnswer();

		FTimerHandle ResponseAnswerTimer;
		FTimerDelegate ResponseAnswerDele;
		ResponseAnswerDele.BindUObject(this, &ARoomEscapeFPSPlayerState::ClientResponseOnResult, bSuccess);
		// TODO: Client RPC로 해당 유저의 UI에서 연출 시작.
		GetWorld()->GetTimerManager().SetTimer(ResponseAnswerTimer, ResponseAnswerDele, 0.2f, false, 0.2f);
	}
}
void ARoomEscapeFPSPlayerState::ClientResponseOnResult_Implementation(bool bSuccess)
{
	if (GetNetMode() == NM_Client)
	{
		// TODO: Client RPC로 해당 유저의 UI에서 연출 시작.
		UPipeGameUI* gameUI = GetUIMgr()->GetPipeGameUI();
		if (gameUI)
		{
			gameUI->CheckCommittedAnswerAnimation(bSuccess);
		}
	}
}

bool ARoomEscapeFPSPlayerState::CheckPipeAnswer()
{
	// 너비 우선 탐색.
	const TArray<FPipeNode>& nodes = PipeGameInfo.GetPipeNodes();
	int32 endPosIndex = (PipeGameInfo.GetGridSize() * PipeGameInfo.GetGridSize()) - 1;

	TQueue<FPipeNode*> answerNodes;
	TSet<FIntPoint> closeNodes;
	answerNodes.Enqueue(const_cast<FPipeNode*>(&nodes[0]));
	while (!answerNodes.IsEmpty())
	{
		FPipeNode* targetNode = nullptr;
		answerNodes.Dequeue(targetNode);
		targetNode->SetAnswerNode(true);

		check(targetNode);
		if (targetNode->GetPipeLocation() == nodes[nodes.Num() - 1].GetPipeLocation())
			return true;

		// 무한루프 방지를 위해서 탐색이 끝난 파이프 노드를 따로 저장/체크한다.
		if (closeNodes.Contains(targetNode->GetPipeLocation()))
			continue;
		closeNodes.Add(targetNode->GetPipeLocation());

		if (PipeGameInfo.IsConnected(*targetNode, EPipeDirection::ERight) &&
			PipeGameInfo.IsConnected(*targetNode, EPipeDirection::EDown))
		{
			int32 index = targetNode->GetPipeLocation().X + 
				(targetNode->GetPipeLocation().Y * PipeGameInfo.GetGridSize()) + 1;
			answerNodes.Enqueue(const_cast<FPipeNode*>(&nodes[index]));
			
			index = targetNode->GetPipeLocation().X +
				(targetNode->GetPipeLocation().Y * PipeGameInfo.GetGridSize()) + PipeGameInfo.GetGridSize();
			answerNodes.Enqueue(const_cast<FPipeNode*>(&nodes[index]));
		}
		else if (PipeGameInfo.IsConnected(*targetNode, EPipeDirection::ERight))
		{
			int32 index = targetNode->GetPipeLocation().X +
				(targetNode->GetPipeLocation().Y * PipeGameInfo.GetGridSize()) + 1;
			answerNodes.Enqueue(const_cast<FPipeNode*>(&nodes[index]));
		}
		else if (PipeGameInfo.IsConnected(*targetNode, EPipeDirection::EDown))
		{
			int32 index = targetNode->GetPipeLocation().X +
				(targetNode->GetPipeLocation().Y * PipeGameInfo.GetGridSize()) + PipeGameInfo.GetGridSize();
			answerNodes.Enqueue(const_cast<FPipeNode*>(&nodes[index]));
		}
	}

	return false;
}

void ARoomEscapeFPSPlayerState::ServerClearPipeGame_Implementation()
{
	if (GetNetMode() == NM_DedicatedServer)
	{
		bPipeGameOpened = false;
		GetUIMgr()->CachPipeGameUI(nullptr);
	}
}
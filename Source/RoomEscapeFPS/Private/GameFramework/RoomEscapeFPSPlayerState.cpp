// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/RoomEscapeFPSPlayerState.h"
#include "GameFramework/RoomEscapeFPSGameState.h"
#include "GameFramework/RoomEscapeFPSPlayerController.h"
#include "GameFramework/RoomEscapeFPSHUD.h"
#include "Character/RoomEscapeFPSCharacter.h"
#include "Gameplay/PipeGameInfo.h"
#include "Helper/Helper.h"
#include "Managers/UIManager.h"
#include "UI/PipeGameUI.h"
#include "UI/InventoryPanel.h"
#include "Kismet/GameplayStatics.h"

#include "Net/UnrealNetwork.h"

#include "Gameplay/TypeInfoHeader.h"

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
	DOREPLIFETIME(ARoomEscapeFPSPlayerState, bInitializePipeGame);
	DOREPLIFETIME(ARoomEscapeFPSPlayerState, PipeGameSuccessInfo);
	DOREPLIFETIME(ARoomEscapeFPSPlayerState, InventoryInfo);
	DOREPLIFETIME(ARoomEscapeFPSPlayerState, fBatteryRemainValue);
	DOREPLIFETIME(ARoomEscapeFPSPlayerState, fBatteryMaxValue);
	DOREPLIFETIME(ARoomEscapeFPSPlayerState, fBatteryUpdateValue);
	DOREPLIFETIME(ARoomEscapeFPSPlayerState, fFlashIntensity);
}
void ARoomEscapeFPSPlayerState::BeginPlay()
{
	Super::BeginPlay();
	if (GetNetMode() == NM_DedicatedServer)
	{
		fBatteryRemainValue = 0.f;
		fBatteryUpdateValue = 1.f;
		fBatteryMaxValue = 300.f;
	}
	//bInitializePipeGame = EReplicateState::EUnknown;
	//PipeGameSuccessInfo = EReplicateState::EUnknown;
}
/*
������ ����:
2x2 ~ 5x5���� �ִ� ť�� ������ ���� ��������,
�� ���(������ Ÿ��)�� ������ ���� ���� �帣�� �Ѵ�.
���� ���ʿ��� ������, ������ �Ʒ��θ� �帣��, �� Ÿ�� ���� ������ �Ʒ��� ����.

1. ���� ��� ���� ����
2. ���� ����� Ÿ�� ���� ȸ�� + ���� �߰�
3. 3������ ������ ���� ���鿡 ������ ȸ���� �ο�.
4. Ŭ���̾�Ʈ���� UI�� ǥ�� ó��
*/

void ARoomEscapeFPSPlayerState::InitializePipeGame(uint8 InGridSize)
{
	/* ���� �˾��� �����⵵ ���� ���� ������� ��û�� ��� bool���� �������� ��� true�� ��찡
	���� �̷��� ���(�̹� true�ε� ����� ��û�� �� ���) �÷��׸� ���� �������ش�. */
	if (bInitializePipeGame == true)
	{
		bInitializePipeGame = false;
		PipeGameSuccessInfo = EReplicateState::EUnknown;
	}

	// ���������� ����(����)
	if (GetNetMode() == NM_DedicatedServer)
	{
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
			nodeInfo[i].SetAnswerNode(true);
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
				if (FMath::RandBool())
				{
					elem.SetRandomDir(FMath::RandRange((int32)0, (int32)2));
				}
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
				if (f < 0.7f)
				{
					elem.SetRandomDir(2);
				}
				else if (f < 0.9f)
				{
					elem.SetRandomDir(3);
				}
				else
				{
					elem.SetRandomDir(4);
				}
			}

			// ���� �÷��� �ʱ�ȭ
			elem.SetAnswerNode(false);
			elem.SetPipeType();
			++nodePos;
		}

		bInitializePipeGame = true;
	}
}
void ARoomEscapeFPSPlayerState::OnRep_InitializePipeGame()
{
	if (bInitializePipeGame == true)
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
		PipeGameSuccessInfo = CheckPipeAnswer();
	}
}
void ARoomEscapeFPSPlayerState::OnRep_PipeGameSuccessInfo()
{
	if (PipeGameSuccessInfo == EReplicateState::EUnknown)
		return;
	
	if (GetNetMode() == NM_Client)
	{
		UPipeGameUI* gameUI = GetUIMgr()->GetPipeGameUI();
		if (gameUI)
		{
			gameUI->CheckCommittedAnswerAnimation(PipeGameSuccessInfo == EReplicateState::ETrue ? true : false);
		}
	}
}

EReplicateState ARoomEscapeFPSPlayerState::CheckPipeAnswer()
{
	// �ʺ� �켱 Ž��.
	if (GetNetMode() == NM_DedicatedServer)
	{
		const TArray<FPipeNode>& nodes = PipeGameInfo.GetPipeNodes();
		int32 endPosIndex = (PipeGameInfo.GetGridSize() * PipeGameInfo.GetGridSize()) - 1;

		TQueue<FPipeNode*> answerNodes;
		TSet<FIntPoint> closeNodes;
		answerNodes.Enqueue(const_cast<FPipeNode*>(&nodes[0]));
		FPipeNode* targetNode = nullptr;
		while (!answerNodes.IsEmpty())
		{
			answerNodes.Dequeue(targetNode);
			targetNode->SetAnswerNode(true);

			check(targetNode);
			if (targetNode->GetPipeLocation() == nodes[nodes.Num() - 1].GetPipeLocation())
			{
				targetNode->SetLastAnswerNode(true);
				return EReplicateState::ETrue;
			}

			// ���ѷ��� ������ ���ؼ� Ž���� ���� ������ ��带 ���� ����/üũ�Ѵ�.
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
			else
			{
				if (PipeGameInfo.IsStartNode(*targetNode))
				{
					targetNode->SetAnswerNode(false);
				}
			}
		}

		if (targetNode)
		{
			targetNode->SetLastAnswerNode(true);
		}
	}
	
	return EReplicateState::EFalse;
}

void ARoomEscapeFPSPlayerState::ServerClearPipeGame_Implementation()
{
	if (GetNetMode() == NM_DedicatedServer)
	{
		bInitializePipeGame = false;
		PipeGameSuccessInfo = EReplicateState::EUnknown;
		//ClientClearPipeGame();
	}
}
//void ARoomEscapeFPSPlayerState::ClientClearPipeGame_Implementation()
//{
//	APawn* pawn = GetPawn();
//	if (pawn && pawn->IsLocallyControlled() && GetNetMode() == NM_Client)
//	{
//		UPipeGameUI* pipeUI = GetUIMgr()->GetPipeGameUI();
//		if (pipeUI)
//		{
//			pipeUI->CloseUI();
//		}
//	}
//}

void ARoomEscapeFPSPlayerState::AddItemToInventory(EItemType InType, uint32 InCount)
{
	bool bFind = false;
	for (int32 i = 0; i < InventoryInfo.Num(); ++i)
	{
		if (InType == InventoryInfo[i].ItemType)
		{
			InventoryInfo[i].ItemCount = InCount;
			bFind = true;
			break;
		}
	}

	if (bFind == false)
	{
		InventoryInfo.Add(FItemInfo(InType, InCount));
		// TODO: ������ ���� ȹ���� ��� ȹ���� �����ۿ� ���� �ȳ� ���� ����.
	}
}
void ARoomEscapeFPSPlayerState::ModifyItemFromInventory(EItemType InType, int32 delta)
{
	bool bFind = false;
	for (int32 i = 0; i < InventoryInfo.Num(); ++i)
	{
		if (InType == InventoryInfo[i].ItemType)
		{
			if (InventoryInfo[i].ItemCount + delta < 0)
			{
				ensureMsgf(false, TEXT("Item StackCount Can't be lower than zero"));
				return;
			}
			InventoryInfo[i].ItemCount += delta;
			break;
		}
	}

	check(bFind);
}

void ARoomEscapeFPSPlayerState::ToggleBatteryReduceState(bool bOnOff)
{
	if (GetNetMode() == NM_DedicatedServer)
	{
		if (bOnOff)
		{
			if (UpdateBatteryDele.IsBound() == false)
			{
				UpdateBatteryDele.BindUObject(this, &ARoomEscapeFPSPlayerState::UpdateBatteryRemainValue, -fBatteryUpdateValue);
			}
			GetWorld()->GetTimerManager().SetTimer(FlashBatteryTimerHandle, UpdateBatteryDele, 1.f, true);
		}
		else
		{
			GetWorld()->GetTimerManager().ClearTimer(FlashBatteryTimerHandle);
		}
	}
}

void ARoomEscapeFPSPlayerState::UpdateBatteryRemainValue(float fDelta)
{
	if (GetNetMode() == NM_DedicatedServer)
	{
		fBatteryRemainValue += fDelta;
		if (fBatteryRemainValue <= 0)
		{
			fBatteryRemainValue = 0.f;
			ToggleBatteryReduceState(false);
		}
	}
}
void ARoomEscapeFPSPlayerState::OnRep_BatteryRemainValue()
{
	// TODO: UI�� ���� ǥ��.
	ARoomEscapeFPSHUD* hud = Cast<ARoomEscapeFPSHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
	if (hud)
	{
		float fPercent = fBatteryRemainValue / fBatteryMaxValue;
		hud->GetInventoryPanel()->UpdateProgressBar(fPercent);
		ARoomEscapeFPSCharacter* character = Cast<ARoomEscapeFPSCharacter>(GetPawn());
		if (character)
		{
			fFlashIntensity = 100000.0f;
			if (fBatteryRemainValue <= 0.f)
			{
				fFlashIntensity = 0.f;
			}
			else if (fBatteryRemainValue <= 100.f)
			{
				fFlashIntensity = 40000.0f;
			}
			character->UpdateFlashIntensity(fFlashIntensity);
		}
	}
}
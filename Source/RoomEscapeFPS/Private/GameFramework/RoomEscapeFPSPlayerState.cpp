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
#include "UI/FirstGetItemInfoPanel.h"
#include "Kismet/GameplayStatics.h"
#include "Object/GetableObject.h"
#include "Components/SceneCaptureComponent2D.h"

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
	DOREPLIFETIME(ARoomEscapeFPSPlayerState, BatteryMaxValue);
	DOREPLIFETIME(ARoomEscapeFPSPlayerState, BatteryUpdateValue);
	DOREPLIFETIME(ARoomEscapeFPSPlayerState, fFlashIntensity);
}
void ARoomEscapeFPSPlayerState::BeginPlay()
{
	Super::BeginPlay();
	if (GetNetMode() == NM_DedicatedServer)
	{
		BatteryUpdateValue = 1;
		BatteryMaxValue = 300u;
	}
	//bInitializePipeGame = EReplicateState::EUnknown;
	//PipeGameSuccessInfo = EReplicateState::EUnknown;
}
/*
������ ����:
�ּ� 2x2, �ִ� nxn���� �ִ� ť�� ������ ���� ��������,
�� ���(������ Ÿ��)�� ������ ���� ���� �帣�� �Ѵ�.
���� ���ʿ��� ������, ������ �Ʒ��θ� �帣��, �� Ÿ�� ���� ������ �Ʒ��� ����.

1. ���� ��� ���� ����
2. ���� ����� Ÿ�� ���� ȸ�� + ���� �߰�
3. 3������ ������ ���� ���鿡 ������ ȸ���� �ο�.
4. Ŭ���̾�Ʈ�� ����, UI ǥ�� ó��
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
		if (PipeGameSuccessInfo == EReplicateState::ETrue)
		{
			ARoomEscapeFPSGameState* gs = Helper::GetGameState(GetWorld());
			if (gs)
			{
				gs->OnCorrectAnswer(EServerSolutionType::EPipelineGame_Complete);
			}
			Helper::UpdateNextUIInfo(GetWorld(), ENextInformationType::EPipelineComplete, ENextInformationType::ERunaway, 1);
		}
	}
}
void ARoomEscapeFPSPlayerState::OnRep_PipeGameSuccessInfo()
{
	if (PipeGameSuccessInfo == EReplicateState::EUnknown)
		return;
	
	if (GetNetMode() == NM_Client)
	{
		ARoomEscapeFPSHUD* hud = Cast<ARoomEscapeFPSHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
		if (hud)
		{
			UPipeGameUI* gameUI = hud->GetPipeGameUI();
			if (gameUI)
			{
				gameUI->CheckCommittedAnswerAnimation(PipeGameSuccessInfo == EReplicateState::ETrue ? true : false);
			}
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
			{	// ���� ��������(Goal Node �����ߴ���) Ȯ��.
				targetNode->SetLastAnswerNode(true);
				return EReplicateState::ETrue;
			}

			// ���ѷ��� ������ ���ؼ� Ž���� ���� ������ ��带 ���� ����/üũ�Ѵ�.
			if (closeNodes.Contains(targetNode->GetPipeLocation()))
				continue;
			closeNodes.Add(targetNode->GetPipeLocation());

			if (PipeGameInfo.IsConnected(*targetNode, EPipeDirection::ERight) &&
				PipeGameInfo.IsConnected(*targetNode, EPipeDirection::EDown))
			{	// ������, �Ʒ� ��� ����Ǿ� ����. ���� ���, ���� ��� ť�� �߰�.
				int32 index = targetNode->GetPipeLocation().X +
					(targetNode->GetPipeLocation().Y * PipeGameInfo.GetGridSize()) + 1;
				answerNodes.Enqueue(const_cast<FPipeNode*>(&nodes[index]));

				index = targetNode->GetPipeLocation().X +
					(targetNode->GetPipeLocation().Y * PipeGameInfo.GetGridSize()) + PipeGameInfo.GetGridSize();
				answerNodes.Enqueue(const_cast<FPipeNode*>(&nodes[index]));
			}
			else if (PipeGameInfo.IsConnected(*targetNode, EPipeDirection::ERight))
			{	// ������ ��常 ����Ǿ� ����. ���� ��� ť�� �߰�.
				int32 index = targetNode->GetPipeLocation().X +
					(targetNode->GetPipeLocation().Y * PipeGameInfo.GetGridSize()) + 1;
				answerNodes.Enqueue(const_cast<FPipeNode*>(&nodes[index]));
			}
			else if (PipeGameInfo.IsConnected(*targetNode, EPipeDirection::EDown))
			{	// �Ʒ��� ��常 ����Ǿ� ����. ���� ��� ť�� �߰�.
				int32 index = targetNode->GetPipeLocation().X +
					(targetNode->GetPipeLocation().Y * PipeGameInfo.GetGridSize()) + PipeGameInfo.GetGridSize();
				answerNodes.Enqueue(const_cast<FPipeNode*>(&nodes[index]));
			}
			else
			{	// ���� ����ε� ���� �带 �� ���� ���(��� ���� ���), ���� �÷��� ���� �� ���� Ż��(Empty Queue)
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

void ARoomEscapeFPSPlayerState::AddItemToInventory(EItemType InType, int32 InCount)
{
	bool bFind = false;
	for (int32 i = 0; i < InventoryInfo.Num(); ++i)
	{
		if (InType == InventoryInfo[i].ItemType)
		{
			InventoryInfo[i].ItemCount += InCount;
			bFind = true;
			break;
		}
	}

	if (bFind == false)
	{
		InventoryInfo.Add(FItemInfo(InType, InCount));
	}
}
const uint32 ARoomEscapeFPSPlayerState::GetItemCount(EItemType InType)
{
	for (int32 i = 0; i < InventoryInfo.Num(); ++i)
	{
		if (InType == InventoryInfo[i].ItemType)
		{
			return InventoryInfo[i].ItemCount;
		}
	}
	return 0u;
}

bool ARoomEscapeFPSPlayerState::AmIHaveItem(EItemType InType)
{
	for (int32 i = 0; i < InventoryInfo.Num(); ++i)
	{
		if (InType == InventoryInfo[i].ItemType)
		{
			return InventoryInfo[i].ItemCount > 0;
		}
	}
	return false;
}
void ARoomEscapeFPSPlayerState::ToggleBatteryReduceState(bool bOnOff)
{
	if (GetNetMode() == NM_DedicatedServer)
	{
		if (bOnOff)
		{
			if (UpdateBatteryDele.IsBound() == false)
			{
				UpdateBatteryDele.BindUObject(this, &ARoomEscapeFPSPlayerState::UpdateBatteryRemainValue, -BatteryUpdateValue);
			}
			GetWorld()->GetTimerManager().SetTimer(FlashBatteryTimerHandle, UpdateBatteryDele, 1.f, true);
		}
		else
		{
			GetWorld()->GetTimerManager().ClearTimer(FlashBatteryTimerHandle);
		}
	}
}
uint32* ARoomEscapeFPSPlayerState::GetItemCountRef(EItemType InType)
{
	for (int32 i = 0; i < InventoryInfo.Num(); ++i)
	{
		if (InType == InventoryInfo[i].ItemType)
		{
			return &InventoryInfo[i].ItemCount;
		}
	}

	return nullptr;
}
void ARoomEscapeFPSPlayerState::UpdateBatteryRemainValue(int32 InDelta)
{
	if (GetNetMode() == NM_DedicatedServer)
	{
		uint32* batteryRemain = GetItemCountRef(EItemType::BatteryPower);
		
		*batteryRemain += InDelta;
		if (*batteryRemain <= 0u)
		{
			*batteryRemain = 0u;
			ToggleBatteryReduceState(false);
		}
		else if (*batteryRemain >= BatteryMaxValue)
		{
			*batteryRemain = BatteryMaxValue;
		}
	}
}

bool ARoomEscapeFPSPlayerState::IsFirstGet(EItemType InType)
{
	for (int32 i = 0; i < InventoryInfo.Num(); ++i)
	{
		if (InType == InventoryInfo[i].ItemType)
			return false;
	}
	return true;
}
void ARoomEscapeFPSPlayerState::ClientProcessHUDOnFirstItemGet_Implementation(class AGetableObject* InObj)
{
	UFirstGetItemInfoPanel* ItemInfoUI = GetUIMgr()->OpenWidget<UFirstGetItemInfoPanel>();
	if (ItemInfoUI)
	{
		ItemInfoUI->SetItemNameText(InObj->GetItemNameStr());
		ItemInfoUI->SetItemDescText(InObj->GetItemDescStr());
	}
	ARoomEscapeFPSHUD* hud = Cast<ARoomEscapeFPSHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
	if (hud && InObj)
	{
		hud->SetVisibleOnHUD(InObj->GetItemType(), true);
	}
}
void ARoomEscapeFPSPlayerState::ClientProcessHUDOnUpdateNextInfo_Implementation(ENextInformationType curType, ENextInformationType nextType, int32 InCount)
{
	ARoomEscapeFPSHUD* hud = Cast<ARoomEscapeFPSHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
	if (hud)
	{
		hud->UpdateNextInfo(curType, nextType, InCount);
	}
}

void ARoomEscapeFPSPlayerState::OnRep_InventoryInfo()
{
	ARoomEscapeFPSHUD* hud = Cast<ARoomEscapeFPSHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
	if (hud)
	{	// ���� ī��Ʈ ������Ʈ(UI)
		hud->GetInventoryPanel()->UpdateCharmCount(GetItemCount(EItemType::Charm));
		
		// ���͸� �ܷ� ������Ʈ(UI)
		uint32 batteryRemain = GetItemCount(EItemType::BatteryPower);
		float fPercent = (float)batteryRemain / BatteryMaxValue;
		hud->GetInventoryPanel()->UpdateBatteryPower(fPercent);

		// ���͸� �ܷ��� ���� ���� ������Ʈ
		ARoomEscapeFPSCharacter* character = Cast<ARoomEscapeFPSCharacter>(GetPawn());
		if (character)
		{
			fFlashIntensity = 100000.0f;
			if (batteryRemain <= 0u)
			{
				fFlashIntensity = 0.f;
			}
			else if (batteryRemain <= 100u)
			{
				fFlashIntensity = 20000.0f;
			}
			character->UpdateFlashIntensity(fFlashIntensity);
		}
	}
}

// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameFramework/RoomEscapeFPSGameMode.h"
#include "GameFramework/RoomEscapeFPSHUD.h"
#include "Character/RoomEscapeFPSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Helper/Helper.h"
#include "UI/InteractionPanel.h"

ARoomEscapeFPSGameMode::ARoomEscapeFPSGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Resources/Character/Blueprints/RoomEscapeFPSCharacter_BP"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ARoomEscapeFPSHUD::StaticClass();
}

void ARoomEscapeFPSGameMode::BeginPlay()
{
	Super::BeginPlay();

	//if (HUDClass)
	//{
	//	HUDClass->AddToRoot();
	//}
}
/*
������ ����:
2x2 ~ 5x5���� �ִ� ť�� ������ ���� ��������,
�� ���(������ Ÿ��)�� ������ ���� ���� �帣�� �Ѵ�.
���� ���ʿ��� ������, ������ �Ʒ��θ� �帣��, �� Ÿ�� ���� ������ �Ʒ��� ����.

1. ���� ��� ���� ����
2. ���� ����� Ÿ�� ���� ����
(1�� ������ �������� �ʴ� ���� ������ ���� ����)
3. �ʺ� �켱 Ž������ �ذ� �����ϴ��� ����
4. 3������ ������ ���� ���鿡 ������ ȸ���� �ο�.
5. Ŭ���̾�Ʈ�� ������ ����.
*/
void ARoomEscapeFPSGameMode::InitializePipeGame(int32 InPlayerID, uint8 GridSize)
{
	// TODO: ��û�� �÷��̾��� GameState���� ó���ϵ��� ���� ����(���� �Լ��� ��� �̰�)
	UPipeGameInfo* pipeInfo = nullptr;
	if (pipeGameHandler.Contains(InPlayerID) == false)
	{
		pipeInfo = NewObject<UPipeGameInfo>();
		check(pipeInfo);
		pipeGameHandler.Add(InPlayerID, pipeInfo);
	}
	else
	{
		pipeInfo = pipeGameHandler[InPlayerID];
	}

	// ���� ������ ������ �޾Ƽ� �ʱ�ȭ�ϰ�, ���� �����̸� �ν��Ͻ�.
	TMap<FIntPoint, FPipeNode>& nodeInfo = pipeInfo->InitializeNodes(GridSize);

	// Ŭ���̾�Ʈ�� ��򰡿��� Server_Replicated�� ȣ��Ǿ� ���������� ���� ����.
	// �� + �� - 1 ������ ���� ��带 �����Ѵ�.
	int32 NeedNodeCount = (GridSize * 2) - 1;
	FIntPoint currentPos(0, 0);
	for (int32 i = 0; i < NeedNodeCount; ++i)
	{
		pipeInfo->AddAnswerNodeInfo(nodeInfo[currentPos]);
		if (currentPos.X >= GridSize - 1 && currentPos.Y >= GridSize - 1)
		{	// Goal��忡 �����ߴ��� üũ
			break;
		}

		bool IsRight = FMath::RandBool();
		if ((currentPos.X >= GridSize - 1 && IsRight) ||
			(currentPos.Y >= GridSize - 1 && !IsRight))
		{
			IsRight = !IsRight;
		}
		// Output ���� �� �� ��������� Input���� ����.
		if (IsRight)
		{	
			nodeInfo[currentPos].AddDirection(EPipeDirection::ERight);
			currentPos.X += 1;
			nodeInfo[currentPos].AddDirection(EPipeDirection::ELeft);
		}
		else
		{
			nodeInfo[currentPos].AddDirection(EPipeDirection::EDown);
			currentPos.Y += 1;
			nodeInfo[currentPos].AddDirection(EPipeDirection::EUp);
		}
	}
	// ���� ����� Input�� �� ����� Output�� ���� ���������� �ٽ� ���ش�.
	nodeInfo[FIntPoint::ZeroValue].AddDirection(FMath::RandBool() ? EPipeDirection::ELeft : EPipeDirection::EUp);
	nodeInfo[FIntPoint(GridSize - 1, GridSize - 1)].AddDirection(FMath::RandBool() ? EPipeDirection::ERight : EPipeDirection::EDown);

	// ���� ������ �Ϸ�Ǿ����� �̵��� �ڼ���, ������ Ÿ���� ���� �����Ѵ�.
	for (auto& elem : nodeInfo)
	{
		// ���� ��� ������ ȸ����Ű�� + ���� �߰��ϱ�(Ȯ����)
		if (elem.Value.IsAnswerNode())
		{
			if (FMath::RandBool())
			{	// 50% Ȯ���� 0 ~ 2���� ������ �߰�
				elem.Value.SetRandomDir(FMath::RandRange((int32)0, (int32)2));
			}
			// ������ ȸ��
			int32 count = FMath::RandRange((int32)1, (int32)3);
			for (int32 i = 0; i < count; ++i)
			{
				elem.Value.RotatePipe();
			}
		}
		else
		{
			// 2����(���� ����) 90% / 3���� 9% / 4���� 1%
			float f = FMath::RandRange(0.f, 1.f);
			if (f < 0.9f)
			{
				elem.Value.SetRandomDir(2);
			}
			else if (f < 0.99f)
			{
				elem.Value.SetRandomDir(3);
			}
			else
			{
				elem.Value.SetRandomDir(4);
			}
		}

		elem.Value.SetPipeType();
	}
}
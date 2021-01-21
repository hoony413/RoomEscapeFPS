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
파이프 게임:
2x2 ~ 5x5까지 있는 큐브 형태의 퍼즐 게임으로,
각 노드(파이프 타일)의 방향을 돌려 물을 흐르게 한다.
물은 왼쪽에서 오른쪽, 위에서 아래로만 흐르며, 맵 타일 생성 로직은 아래와 같다.

1. 정답 경로 랜덤 생성
2. 정답 경로의 타일 랜덤 생성
(1번 순서를 위배하지 않는 범위 내에서 랜덤 생성)
3. 너비 우선 탐색으로 해가 존재하는지 검증
4. 3번에서 추출한 정답 노드들에 랜덤한 회전값 부여.
5. 클라이언트로 데이터 전송.
*/
void ARoomEscapeFPSGameMode::InitializePipeGame(int32 InPlayerID, uint8 GridSize)
{
	// TODO: 요청한 플레이어의 GameState에서 처리하도록 로직 변경(관련 함수도 모두 이관)
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

	// 기존 정보가 있으면 받아서 초기화하고, 최초 실행이면 인스턴스.
	TMap<FIntPoint, FPipeNode>& nodeInfo = pipeInfo->InitializeNodes(GridSize);

	// 클라이언트의 어딘가에서 Server_Replicated로 호출되어 파이프게임 생성 시작.
	// 행 + 열 - 1 개수의 정답 노드를 생성한다.
	int32 NeedNodeCount = (GridSize * 2) - 1;
	FIntPoint currentPos(0, 0);
	for (int32 i = 0; i < NeedNodeCount; ++i)
	{
		pipeInfo->AddAnswerNodeInfo(nodeInfo[currentPos]);
		if (currentPos.X >= GridSize - 1 && currentPos.Y >= GridSize - 1)
		{	// Goal노드에 도착했는지 체크
			break;
		}

		bool IsRight = FMath::RandBool();
		if ((currentPos.X >= GridSize - 1 && IsRight) ||
			(currentPos.Y >= GridSize - 1 && !IsRight))
		{
			IsRight = !IsRight;
		}
		// Output 설정 후 그 다음노드의 Input으로 연결.
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
	// 시작 노드의 Input과 골 노드의 Output은 따로 랜덤설정을 다시 해준다.
	nodeInfo[FIntPoint::ZeroValue].AddDirection(FMath::RandBool() ? EPipeDirection::ELeft : EPipeDirection::EUp);
	nodeInfo[FIntPoint(GridSize - 1, GridSize - 1)].AddDirection(FMath::RandBool() ? EPipeDirection::ERight : EPipeDirection::EDown);

	// 정답 생성이 완료되었으면 이들을 뒤섞고, 나머지 타일을 랜덤 생성한다.
	for (auto& elem : nodeInfo)
	{
		// 정답 노드 무작위 회전시키기 + 방향 추가하기(확률성)
		if (elem.Value.IsAnswerNode())
		{
			if (FMath::RandBool())
			{	// 50% 확률로 0 ~ 2개의 방향을 추가
				elem.Value.SetRandomDir(FMath::RandRange((int32)0, (int32)2));
			}
			// 파이프 회전
			int32 count = FMath::RandRange((int32)1, (int32)3);
			for (int32 i = 0; i < count; ++i)
			{
				elem.Value.RotatePipe();
			}
		}
		else
		{
			// 2방향(직선 포함) 90% / 3방향 9% / 4방향 1%
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
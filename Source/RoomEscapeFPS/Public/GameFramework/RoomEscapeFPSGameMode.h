// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RoomEscapeFPSGameMode.generated.h"

/* 파이프게임 관련 */
// Up, Left는 Input 전용이고 Down, Right은 Output 전용.
UENUM()
enum class EPipeDirection : uint8
{
	EUp = 0x01, 
	ERight = 0x02,
	EDown = 0x04,
	ELeft = 0x08,
	EMAX = 0x10,
};

UENUM()
enum class EPipeType : uint8
{
	EStraight_Two = 0u,
	ETwo = 1u,
	EThree = 2u,
	EFour = 3u,
};

// 파이프 노드 정보
USTRUCT()
struct ROOMESCAPEFPS_API FPipeNode
{
	GENERATED_BODY()

public:
	FPipeNode() {}

	bool IsContainDirection(EPipeDirection InDir)
	{
		return (uint8)InDir == ((uint8)InDir & (uint8)DirectionInfo);
	}

	void AddDirection(EPipeDirection InDir)
	{
		DirectionInfo += (uint8)InDir;
	}

	FORCEINLINE bool IsAnswerNode() const { return bAnswerNode; }
	FORCEINLINE void SetAnswerNode(bool bAnswer) { bAnswerNode = bAnswer; }
	void SetRandomDir(int32 max)
	{
		int32 curCount = 0;
		while (true)
		{
			if (curCount >= max)
				break;
			int32 randValue = FMath::RandRange((int32)0, (int32)3);
			if (!IsContainDirection((EPipeDirection)(1 << randValue)))
			{
				AddDirection((EPipeDirection)(1 << randValue));
				++curCount;
			}
		}
	}

	FORCEINLINE void RotatePipe()
	{
		// 비트 회전 연산
		DirectionInfo = _rotl(DirectionInfo, 1);
		if (DirectionInfo > (uint8)EPipeDirection::EMAX)
		{
			DirectionInfo -= (uint8)EPipeDirection::EMAX;
			DirectionInfo |= (uint8)EPipeDirection::EUp;
		}
	}

	FORCEINLINE EPipeType GetPipeType() const { return PipeType; }
	void SetPipeType()
	{
		int32 dirCount = 0;
		for (int32 i = 0; i < 4; ++i)
		{
			if ((DirectionInfo & 1 << i) == 1 << i)
				++dirCount;
		}

		if (dirCount == 2)
		{
			PipeType = DirectionInfo % 5u == 0 ? EPipeType::EStraight_Two : EPipeType::ETwo;
		}
		else if (dirCount == 3)
		{
			PipeType = EPipeType::EThree;
		}
		else if (dirCount == 4)
		{
			PipeType = EPipeType::EFour;
		}
	}

private:
	uint8 DirectionInfo = 0u;
	bool bAnswerNode = false;
	EPipeType PipeType;
};

// 파이프게임에 필요한 정보
UCLASS()
class ROOMESCAPEFPS_API UPipeGameInfo : public UObject
{
	GENERATED_BODY()

public:
	UPipeGameInfo() {}
	
	TMap<FIntPoint, FPipeNode>& InitializeNodes(uint8 InGridSize)
	{
		PipeNodes.Empty();
		AnswerNodesInfo.Empty();

		for (int32 i = 0; i < InGridSize; ++i)
		{
			for (int32 j = 0; j < InGridSize; ++j)
			{
				PipeNodes.Add(FIntPoint(i, j), FPipeNode());
			}
		}
		return PipeNodes;
	}

	const TQueue<FPipeNode>& GetAnswerNodesInfo()
	{
		return AnswerNodesInfo;
	}

	void AddAnswerNodeInfo(FPipeNode& InNode)
	{
		AnswerNodesInfo.Enqueue(InNode);
		InNode.SetAnswerNode(true);
	}

private:
	UPROPERTY()
	TMap<FIntPoint, FPipeNode> PipeNodes;
	UPROPERTY()
	TQueue<FPipeNode> AnswerNodesInfo;
};

UCLASS(minimalapi)
class ARoomEscapeFPSGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ARoomEscapeFPSGameMode();

	virtual void BeginPlay() override;

	// 파이프게임 생성(기존 데이터를 날리고 새로 생성한다)
	void InitializePipeGame(int32 InPlayerID, uint8 GridSize);

private:
	/* 주의: 키값으로 쓰일 int32는 PlayerID값인데, 이 값은 게임스테이트에서 받아올 수 있다.
	반드시 게임모드(서버에만 있는)->게임스테이트->각 플레이어 access->GetPlayerID 로 처리해야
	일관성을 유지할 수 있음. */
	UPROPERTY()
		TMap<int32, UPipeGameInfo*> pipeGameHandler;
};




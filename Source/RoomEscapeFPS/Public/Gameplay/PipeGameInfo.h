// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ReplicateObject.h"
#include "PipeGameInfo.generated.h"

/**
 * 
 */

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
	EMAX = 4u,
};

// 파이프 노드 정보
USTRUCT()
struct ROOMESCAPEFPS_API FPipeNode
{
	GENERATED_BODY()

public:
	FPipeNode() {}

	FORCEINLINE bool IsContainDirection(EPipeDirection InDir) const
	{
		return (uint8)InDir == ((uint8)InDir & (uint8)DirectionInfo);
	}
	FORCEINLINE void AddDirection(EPipeDirection InDir)
	{
		DirectionInfo += (uint8)InDir;
	}
	FORCEINLINE void SetPipeLocation(const FIntPoint& InLocation)
	{
		PipeLocation = InLocation;
	}
	FORCEINLINE const FIntPoint& GetPipeLocation() const
	{
		return PipeLocation;
	}
	FORCEINLINE uint8 GetDirectionInfo() { return DirectionInfo; }
	FORCEINLINE bool IsAnswerNode() const { return bAnswerNode; }
	FORCEINLINE void SetAnswerNode(const bool bAnswer) { bAnswerNode = bAnswer; }
	FORCEINLINE bool IsLastAnswerNode() const { return bLastAnswerNode; }
	FORCEINLINE void SetLastAnswerNode(const bool bAnswer) { bLastAnswerNode = bAnswer; }

	void SetRandomDir(const int32 max)
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

	uint8 RotatePipe()
	{
		// 비트 회전 연산
		DirectionInfo = _rotl(DirectionInfo, 1);
		if (DirectionInfo > (uint8)EPipeDirection::EMAX)
		{
			DirectionInfo -= (uint8)EPipeDirection::EMAX;
			DirectionInfo |= (uint8)EPipeDirection::EUp;
		}

		return DirectionInfo;
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
	UPROPERTY()
	FIntPoint PipeLocation;
	UPROPERTY()
	uint8 DirectionInfo = 0u;
	UPROPERTY()
	bool bAnswerNode = false;
	// 플레이어가 제출한 파이프라인에서, 물이 흐를 때 최종 도착지가 되는 노드인지?
	UPROPERTY()
	bool bLastAnswerNode = false;
	UPROPERTY()
	EPipeType PipeType;
};

 // 파이프게임에 필요한 정보
USTRUCT()
struct ROOMESCAPEFPS_API FPipeGameInfo
{
	GENERATED_BODY()

public:
	FPipeGameInfo()
	{
	}

	TArray<FPipeNode>& InitializeNodes(uint8 InGridSize)
	{
		PipeNodes.Empty();
		for (int32 i = 0; i < InGridSize; ++i)
		{
			for (int32 j = 0; j < InGridSize; ++j)
			{
				PipeNodes.Add(FPipeNode());
			}
		}
		return PipeNodes;
	}

	TArray<FPipeNode>& GetPipeNodes() { return PipeNodes; }
	FORCEINLINE uint8 GetGridSize() const { return GridSize; }
	FORCEINLINE void SetGridSize(uint8 InGridSize) { GridSize = InGridSize; }

	// 인자로 들어온 노드가 인자로 들어온 방향으로 연결되어 있는지?
	bool IsConnected(FPipeNode& InNode, EPipeDirection InDir)
	{
		bool bConnect = false;
		uint8 reverseDir = _rotl((uint8)InDir, 2);
		if (reverseDir >= (uint8)EPipeDirection::EMAX)
		{
			reverseDir /= (uint8)EPipeDirection::EMAX;
		}

		const FPipeNode& nearbyNode = GetNearbyNode(InNode, InDir);
		if (IsStartNode(InNode))
		{
			return InNode.IsContainDirection(EPipeDirection::ELeft) && 
				InNode.IsContainDirection(InDir) && nearbyNode.IsContainDirection((EPipeDirection)reverseDir);
		}
		else if (IsGoalNode(InNode))
		{
			return InNode.IsContainDirection(EPipeDirection::ERight) &&
				InNode.IsContainDirection(InDir) && nearbyNode.IsContainDirection((EPipeDirection)reverseDir);
		}

		// 인접 노드가 자기자신이다(그리드 범위를 벗어남)
		if (nearbyNode.GetPipeLocation() == InNode.GetPipeLocation())
			return false;
		switch(InDir)
		{
		case EPipeDirection::EUp:
			bConnect = InNode.IsContainDirection(EPipeDirection::EUp)
				&& nearbyNode.IsContainDirection(EPipeDirection::EDown);
			break;
		case EPipeDirection::ERight:
			bConnect = InNode.IsContainDirection(EPipeDirection::ERight)
				&& nearbyNode.IsContainDirection(EPipeDirection::ELeft)
				&& !IsFarRightNode(InNode);
			break;
		case EPipeDirection::EDown:
			bConnect = InNode.IsContainDirection(EPipeDirection::EDown)
				&& nearbyNode.IsContainDirection(EPipeDirection::EUp);
			break;
		case EPipeDirection::ELeft:
			bConnect = InNode.IsContainDirection(EPipeDirection::ELeft)
				&& nearbyNode.IsContainDirection(EPipeDirection::ERight)
				&& !IsFarLeftNode(InNode);
			break;
		default: 
			break;
		}

		return bConnect;
	}

	bool IsStartNode(FPipeNode& InNode)
	{
		return InNode.GetPipeLocation().X == 0 && InNode.GetPipeLocation().Y == 0;
	}
	bool IsGoalNode(FPipeNode& InNode)
	{
		return InNode.GetPipeLocation().X == GridSize - 1 &&
			InNode.GetPipeLocation().Y == GridSize - 1;
	}

private:
	// 인자로 들어온 노드의 인접 노드 구하기(노드 기준의 방향으로)
	FPipeNode& GetNearbyNode(FPipeNode& curNode, EPipeDirection nearbyDir)
	{
		const FIntPoint& pos = curNode.GetPipeLocation();
		int32 curIndex = (pos.Y * GridSize) + pos.X;
		int32 nearbyIndex = curIndex;
		switch (nearbyDir)
		{
		case EPipeDirection::EUp:
			nearbyIndex = curIndex - GridSize;
			break;
		case EPipeDirection::ERight:
			nearbyIndex = curIndex + 1;
			break;
		case EPipeDirection::EDown:
			nearbyIndex = curIndex + GridSize;
			break;
		case EPipeDirection::ELeft:
			nearbyIndex = curIndex - 1;
			break;
		default: break;
		}

		// 그리드 범위를 벗어난 경우 자기자신을 리턴.
		if (nearbyIndex >= (GridSize * GridSize) || nearbyIndex < 0)
			return curNode;
		return PipeNodes[nearbyIndex];
	}

	// 그리드 상에서 가장 왼쪽 노드와 오른쪽 노드는 별도 확인이 필요하다.
	// 인덱스 기준으로는 -1, +1을 하면 되지만, 위치상으로는 다음 라인에 그려지기 때문.
	bool IsFarLeftNode(FPipeNode& InNode)
	{
		int32 index = InNode.GetPipeLocation().X + (InNode.GetPipeLocation().Y * GridSize);
		return index % GridSize == 0;
	}
	bool IsFarRightNode(FPipeNode& InNode)
	{
		int32 index = InNode.GetPipeLocation().X + (InNode.GetPipeLocation().Y * GridSize);
		return index % GridSize == (GridSize - 1);
	}
private:
	UPROPERTY()
		TArray<FPipeNode> PipeNodes;
	UPROPERTY()
		uint8 GridSize = 0u;
};

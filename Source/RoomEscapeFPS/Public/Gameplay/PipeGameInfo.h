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
	FORCEINLINE bool IsCorrectDirection(uint8 InDir)
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
	UPROPERTY()
	FIntPoint PipeLocation;
	UPROPERTY()
	uint8 DirectionInfo = 0u;
	UPROPERTY()
	bool bAnswerNode = false;
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

	void AddAnswerNodeInfo(FPipeNode& InNode)
	{
		InNode.SetAnswerNode(true);
	}

	TArray<FPipeNode>& GetPipeNodes() { return PipeNodes; }
	FORCEINLINE uint8 GetGridSize() const { return GridSize; }
	FORCEINLINE void SetGridSize(uint8 InGridSize) { GridSize = InGridSize; }

private:
	UPROPERTY()
		TArray<FPipeNode> PipeNodes;
	UPROPERTY()
		uint8 GridSize = 0u;
};

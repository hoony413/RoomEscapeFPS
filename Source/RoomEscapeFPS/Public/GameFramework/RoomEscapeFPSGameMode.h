// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Gameplay/TypeInfoHeader.h"
#include "RoomEscapeFPSGameMode.generated.h"

/*
 * 게임모드: 모든 플레이어의 게임스테이트를 소유하고, 게임스테이트를 스폰한다.
 클라이언트에선 접근 불가능, 서버에서만 접근할 수 있다.
 (그래서 중요한 게임 로직들은 서버 RPC에서 호출하여 GameMode를 access하여 호출하는 구조로
 가는게 좋다)

서버에서 모든 플레이어의 PlayerState를 참조하는 소스는 아래와 같이 작성할 수 있다.
AGameModeBase* gMode = GetWorld()->GetAuthGameMode();
if (gMode)
{
	ARoomEscapeFPSGameMode* g = Cast<ARoomEscapeFPSGameMode>(gMode);
	if (g)
	{
		const TArray<APlayerState*>& arr = g->GameState->PlayerArray;
		for (const auto& elem : arr)
		{

		}
	}
}
 */
UCLASS(minimalapi)
class ARoomEscapeFPSGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ARoomEscapeFPSGameMode();

	void BeginPlay() override;

	// 플레이어 접속 시 처리할 액션.
	void PostLogin(APlayerController* NewPlayer) override;

	// 1번 단서의 답: 170 : 1010 1010
	// 2번 단서의 답: 132(서 동 남)
	bool CheckAnswer(const int32 answer, EServerSolutionType InType);

	// 유령잡기 미니게임 온/오프(AGhostSpawner Trigger 동작)
	void SetActiveGhostSpawner(bool bActive);

	FORCEINLINE int32 GetFindLanternCompleteCount() const { return FindLanternCompleteCount; }
	FORCEINLINE int32 GetSolveClue1CompleteCount()  const { return SolveClue1CompleteCount; }
	FORCEINLINE int32 GetSolveClue2CompleteCount()  const { return SolveClue2CompleteCount; }
	FORCEINLINE int32 GetGhostDeadCountTarget()     const { return GhostDeadCount_Target; }
	FORCEINLINE int32 GetPipelineCompleteCount()    const { return PipelineCompleteCount; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameSettings|Answers")
	int32 Clue_1_Answer = 170;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameSettings|Answers")
	int32 Clue_2_Answer = 132;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameSettings|CompleteCounts")
	int32 GhostDeadCount_Target = 10;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameSettings|CompleteCounts")
	int32 FindLanternCompleteCount = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameSettings|CompleteCounts")
	int32 SolveClue1CompleteCount = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameSettings|CompleteCounts")
	int32 SolveClue2CompleteCount = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameSettings|CompleteCounts")
	int32 PipelineCompleteCount = 1;
};




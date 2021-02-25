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

	virtual void BeginPlay() override;

	// 플레이어 접속 시 처리할 액션.
	virtual void PostLogin(APlayerController* NewPlayer) override;

	// 1번 단서의 답: 170 : 1010 1010
	// 2번 단서의 답: 132(서 동 남)
	bool CheckAnswer(const int32 answer, EServerSolutionType InType);

	// 게임 목표 설정(모든 플레이어는 이 값을 참조하여 승리 조건을 알아야 한다)

protected:
	int32 Clue_1_Answer = 170;
	int32 Clue_2_Answer = 132;
};




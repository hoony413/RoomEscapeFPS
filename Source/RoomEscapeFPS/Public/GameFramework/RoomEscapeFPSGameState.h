// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Gameplay/TypeInfoHeader.h"
#include "RoomEscapeFPSGameState.generated.h"

class AInteractiveObject;

/**
 * GameState: 플레이어가 접속하면 GameState가 인스턴스한다.
 GameState는 클라이언트/서버 모두 접근 가능하며, 따라서 게임의 현재 상태(말그대로 GameState)
 를 업데이트하는 목적을 가진 클래스이다.
 GameState의 값을 참조하여 각 플레이어(Client)가 현재의 게임상태에 맞게 동작을 수행할 수
 있도록 돕는다.
 */

UCLASS()
class ROOMESCAPEFPS_API ARoomEscapeFPSGameState : public AGameStateBase
{
	GENERATED_BODY()

		
public:
	void OnCorrectAnswer(EServerSolutionType InType);

	void AddToSolutionResultObject(AInteractiveObject* InObj);
	void SetActiveGhostSpawner(bool bActive);

	void IncreaseGhostDeadCount();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	void BeginPlay() override;
	AInteractiveObject* FindResultActor(EServerSolutionResultType InType);

	UFUNCTION(NetMulticast, Reliable)
void NetMulticastOpenDoorNotice();

protected:
	UPROPERTY(Transient)
	TArray<TObjectPtr<AInteractiveObject>> OnSolutionResultObject;

	UPROPERTY(Replicated)
	int32 GhostDeadCount = 0;
};

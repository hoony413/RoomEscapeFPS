// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Gameplay/TypeInfoHeader.h"
#include "RoomEscapeFPSGameState.generated.h"

/**
 * GameState: �÷��̾ �����ϸ� GameState�� �ν��Ͻ��Ѵ�.
 GameState�� Ŭ���̾�Ʈ/���� ��� ���� �����ϸ�, ���� ������ ���� ����(���״�� GameState)
 �� ������Ʈ�ϴ� ������ ���� Ŭ�����̴�.
 GameState�� ���� �����Ͽ� �� �÷��̾�(Client)�� ������ ���ӻ��¿� �°� ������ ������ ��
 �ֵ��� ���´�.
 */

UCLASS()
class ROOMESCAPEFPS_API ARoomEscapeFPSGameState : public AGameStateBase
{
	GENERATED_BODY()

		
public:
	void OnCorrectAnswer(EServerSolutionType InType);

	void AddToSolutionResultObject(class AInteractiveObject* InObj);
	void SetActiveGhostSpawner(bool bActive);

	UFUNCTION(Server, Reliable)
	void ServerIncreaseGhostDeadCount();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;
	class AInteractiveObject* FindResultActor(EServerSolutionResultType InType);

	UFUNCTION(NetMulticast, Unreliable)
		void NetMulticastOpenDoorNotice();

protected:
	UPROPERTY()
	TArray<class AInteractiveObject*> OnSolutionResultObject;
	UPROPERTY(Replicated)
		int32 GhostDeadCount = 0;
};

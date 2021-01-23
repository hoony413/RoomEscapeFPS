// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
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
	// ���������� �����͸� PlayerState�� �Ѱ��ֱ⸸ ����.
	// (���������� ���� ������ �÷��̾� ���� �����̹Ƿ�)
	void HandoverPipeGameInfo(const int32 inID, struct FPipeGameInfo* nodeInfo);
};

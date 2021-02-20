// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RoomEscapeFPSGameMode.generated.h"

/*
 * ���Ӹ��: ��� �÷��̾��� ���ӽ�����Ʈ�� �����ϰ�, ���ӽ�����Ʈ�� �����Ѵ�.
 Ŭ���̾�Ʈ���� ���� �Ұ���, ���������� ������ �� �ִ�.
 (�׷��� �߿��� ���� �������� ���� RPC���� ȣ���Ͽ� GameMode�� access�Ͽ� ȣ���ϴ� ������
 ���°� ����)

�������� ��� �÷��̾��� PlayerState�� �����ϴ� �ҽ��� �Ʒ��� ���� �ۼ��� �� �ִ�.
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

	// �÷��̾� ���� �� ó���� �׼�.
	virtual void PostLogin(APlayerController* NewPlayer) override;

	// ���� ��ǥ ����(��� �÷��̾�� �� ���� �����Ͽ� �¸� ������ �˾ƾ� �Ѵ�)
};




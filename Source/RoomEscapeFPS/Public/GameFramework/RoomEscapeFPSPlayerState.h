// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Gameplay/PipeGameInfo.h"
#include "RoomEscapeFPSPlayerState.generated.h"

/**
 * �÷��̾����Ʈ
 PlayerController�� ������ ��(�÷��̾� ���� �Ϸ� ��) ���� �����ȴ�.
 PlayerState�� ����/Ŭ���̾�Ʈ ��� ���� �����ѵ�, ������ ��� ��� �÷��̾��� PlayerState��
 ���� �����ϸ�, Ŭ���̾�Ʈ�� �ڱ��ڽ��� PlayerState�� ���� �����ϴ�.
 (�׵� �׷� ����, Ŭ���̾�Ʈ�� GameMode ���� �Ұ����̹Ƿ�)
 */

UCLASS()
class ROOMESCAPEFPS_API ARoomEscapeFPSPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	//virtual bool ReplicateSubobjects(UActorChannel *Channel, FOutBunch *Bunch, FReplicationFlags *RepFlags) override;
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// ���������� ����
	void InitializePipeGame(uint8 GridSize);
	
	UFUNCTION()
		void OnRep_PipeGameInfo();

	UFUNCTION(Server, Reliable)
		void ServerCheckCommittedAnswer();

private:
	UPROPERTY(ReplicatedUsing = OnRep_PipeGameInfo)
		struct FPipeGameInfo PipeGameInfo;

	UFUNCTION()
		bool CheckPipeAnswer();
};

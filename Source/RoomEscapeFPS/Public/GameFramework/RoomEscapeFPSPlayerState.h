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
UENUM()
enum class EReplicateState : uint8
{
	EFalse = 0,
	ETrue = 1,
	EUnknown = 2,
};
UCLASS()
class ROOMESCAPEFPS_API ARoomEscapeFPSPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	//virtual bool ReplicateSubobjects(UActorChannel *Channel, FOutBunch *Bunch, FReplicationFlags *RepFlags) override;
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void BeginPlay() override;
	// ���������� ����
	void InitializePipeGame(uint8 GridSize);
	
	UFUNCTION()
		void OnRep_InitializePipeGame();
	UFUNCTION()
		void OnRep_PipeGameSuccessInfo();

	UFUNCTION(Server, Reliable)
		void ServerRotatePipe(int32 Index);

	UFUNCTION(Server, Reliable)
		void ServerCheckCommittedAnswer();

	UFUNCTION(Server, Reliable)
		void ServerClearPipeGame();
	UFUNCTION(Client, Reliable)
		void ClientClearPipeGame();

	FORCEINLINE struct FPipeGameInfo& GetPipeGameInfo() { return PipeGameInfo; }

private:
	UFUNCTION()
		EReplicateState CheckPipeAnswer();

private:
		
	UPROPERTY(Replicated)
		struct FPipeGameInfo PipeGameInfo;

	UPROPERTY(ReplicatedUsing = OnRep_InitializePipeGame)
		bool bInitializePipeGame = false;

	UPROPERTY(ReplicatedUsing = OnRep_PipeGameSuccessInfo)
		EReplicateState PipeGameSuccessInfo = EReplicateState::EUnknown;
};

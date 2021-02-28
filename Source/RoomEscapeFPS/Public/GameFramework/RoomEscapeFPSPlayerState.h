// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Gameplay/PipeGameInfo.h"
#include "Gameplay/TypeInfoHeader.h"
#include "RoomEscapeFPSPlayerState.generated.h"

/**
 * �÷��̾����Ʈ
 PlayerController�� ������ ��(�÷��̾� ���� �Ϸ� ��) ���� �����ȴ�.
 PlayerState�� ����/Ŭ���̾�Ʈ ��� �����ϴµ�(���ø�����Ʈ), ������ ��� ��� �÷��̾��� PlayerState��
 ���� �����ϸ�, Ŭ���̾�Ʈ�� �ڱ��ڽ��� PlayerState�� ���� �����ϴ�.
 */
UENUM()
enum class EReplicateState : uint8
{
	EFalse = 0,
	ETrue = 1,
	EUnknown = 2,
};

USTRUCT()
struct ROOMESCAPEFPS_API FItemInfo
{
	GENERATED_BODY()

public:
	FItemInfo() {}
	FItemInfo(EItemType InType, uint32 InCount)
	{
		ItemType = InType;
		ItemCount = InCount;
	}

	UPROPERTY()
	EItemType ItemType;
	UPROPERTY()
	uint32 ItemCount;
};

UCLASS()
class ROOMESCAPEFPS_API ARoomEscapeFPSPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	//virtual bool ReplicateSubobjects(UActorChannel *Channel, FOutBunch *Bunch, FReplicationFlags *RepFlags) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void BeginPlay() override;

//--------------------------------------- ���������� ����
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
	//UFUNCTION(Client, Reliable)
	//	void ClientClearPipeGame();

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

//--------------------------------------- ���������� ����

//--------------------------------------- ������ ����

public:
	void AddItemToInventory(EItemType InType, int32 InCount);
	const uint32 GetItemCount(EItemType InType);

	uint32* GetItemCountRef(EItemType InType);
	//uint32& GetItemCountRef(EItemType InType);
	
	bool AmIHaveItem(EItemType InType);

	void ToggleBatteryReduceState(bool bOnOff);

	bool IsFirstGet(EItemType InType);

	UFUNCTION(Client, Unreliable)
	void ClientProcessHUDOnFirstItemGet(class AGetableObject* InObj);
	UFUNCTION(Client, Unreliable)
		void ClientProcessHUDOnUpdateNextInfo(ENextInformationType curType, ENextInformationType nextType, int32 InCount);

protected:
	void UpdateBatteryRemainValue(int32 InDelta);

	UFUNCTION()
		void OnRep_InventoryInfo();

	UPROPERTY(ReplicatedUsing = OnRep_InventoryInfo)
	TArray<FItemInfo> InventoryInfo;

	UPROPERTY(Replicated)
		uint32 BatteryMaxValue;
	UPROPERTY(Replicated)
		int32 BatteryUpdateValue;
	UPROPERTY(Replicated)
		float fFlashIntensity;

	FTimerHandle FlashBatteryTimerHandle;
	FTimerDelegate UpdateBatteryDele;

//--------------------------------------- ������ ����

//--------------------------------------- UI ������Ʈ ����

};

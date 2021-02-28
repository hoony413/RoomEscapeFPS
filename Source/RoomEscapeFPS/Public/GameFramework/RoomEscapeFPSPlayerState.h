// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Gameplay/PipeGameInfo.h"
#include "Gameplay/TypeInfoHeader.h"
#include "RoomEscapeFPSPlayerState.generated.h"

/**
 * 플레이어스테이트
 PlayerController가 스폰될 때(플레이어 접속 완료 시) 같이 스폰된다.
 PlayerState는 서버/클라이언트 모두 존재하는데(리플리케이트), 서버의 경우 모든 플레이어의 PlayerState를
 참조 가능하며, 클라이언트는 자기자신의 PlayerState만 참조 가능하다.
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

//--------------------------------------- 파이프게임 관련
	// 파이프게임 생성
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

//--------------------------------------- 파이프게임 관련

//--------------------------------------- 아이템 관련

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

//--------------------------------------- 아이템 관련

//--------------------------------------- UI 업데이트 관련

};

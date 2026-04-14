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
	NONE = 0 UMETA(Hidden),
	NOT_YET,
	COMPLETE,
	UNKNOWN,
	MAX UMETA(Hidden)
};

USTRUCT()
struct ROOMESCAPEFPS_API FInventoryItemInfo
{
	GENERATED_BODY()

	FInventoryItemInfo() : ItemType(EItemType::NONE), ItemCount(0u) {}
	FInventoryItemInfo(EItemType InType, int32 InCount) : ItemType(InType), ItemCount(InCount) {}

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
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void BeginPlay() override;

//--------------------------------------- 파이프게임 관련
	// 파이프게임 생성
	void InitializePipeGame(uint8 GridSize);
	
	UFUNCTION()
		void OnRep_InitializePipeGame();
	UFUNCTION()
		void OnRep_PipeGameSuccessInfo();

	FORCEINLINE FPipeGameInfo& GetPipeGameInfo() { return PipeGameInfo; }
	FORCEINLINE FPipeGameInfo const& GetPipeGameInfo() const { return PipeGameInfo; }

	void RotatePipe(int32 Index);
	void CheckAndApplyPipeAnswer();
	void ClearPipeGame();

private:
	UFUNCTION()
	EReplicateState CheckPipeAnswer();

private:
	UPROPERTY(Replicated)
	FPipeGameInfo PipeGameInfo;

	UPROPERTY(ReplicatedUsing = OnRep_InitializePipeGame)
	bool bInitializePipeGame{};

	UPROPERTY(ReplicatedUsing = OnRep_PipeGameSuccessInfo)
	EReplicateState PipeGameSuccessInfo{EReplicateState::UNKNOWN};

//--------------------------------------- 파이프게임 관련

//--------------------------------------- 아이템 관련

public:
	void AddItemToInventory(EItemType InType, int32 InCount);
	uint32 GetItemCount(EItemType InType);

	uint32* GetItemCountRef(EItemType InType);
	//uint32& GetItemCountRef(EItemType InType);
	
	bool AmIHaveItem(EItemType InType);

	void ToggleBatteryReduceState(bool bOnOff);

	bool IsFirstGet(EItemType InType);

protected:
	void UpdateBatteryRemainValue(int32 InDelta);
	void UpdateFlashIntensityByBattery();

	UFUNCTION()
		void OnRep_InventoryInfo();

	UFUNCTION()
		void OnRep_FlashIntensity();

	UPROPERTY(ReplicatedUsing = OnRep_InventoryInfo)
	TArray<FInventoryItemInfo> InventoryInfo;

	UPROPERTY(Replicated)
	uint32 BatteryMaxValue;

	UPROPERTY(Replicated)
	int32 BatteryUpdateValue;

	UPROPERTY(ReplicatedUsing = OnRep_FlashIntensity)
	float fFlashIntensity;

	FTimerHandle FlashBatteryTimerHandle;
	FTimerDelegate UpdateBatteryDele;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/InteractiveObject.h"
#include "Gameplay/TypeInfoHeader.h"
#include "GetableObject.generated.h"

/**
 * 획득 시 아이템 인벤토리에 추가되는 아이템.
 */
UCLASS()
class ROOMESCAPEFPS_API AGetableObject : public AInteractiveObject
{
	GENERATED_BODY()

public:
	AGetableObject();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void ToggleState(APawn* requester) override;
	virtual void CaptureCurrentScene();

	FORCEINLINE const FString& GetItemNameStr() { return ItemNameStr; }
	FORCEINLINE const FString& GetItemDescStr() { return ItemDescStr; }

	FORCEINLINE bool IsFirstGetNeedsUpdateUI() { return bFirstGetNeedsUpdateUI; }
	FORCEINLINE EItemType GetItemType() { return ItemType; }
	FORCEINLINE USceneCaptureComponent2D* GetSceneCapture() { return SceneCapturer; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, Category = ItemInfo, meta = (AllowPrivateAccess = "true"))
		EItemType ItemType;
	UPROPERTY(EditAnywhere, Category = ItemInfo, meta = (AllowPrivateAccess = "true", UIMin = "1"))
		int32 DefaultGetCount;
	UPROPERTY(EditAnywhere, Category = ItemInfo, meta = (AllowPrivateAccess = "true"))
		class USceneCaptureComponent2D* SceneCapturer;

	UPROPERTY(EditAnywhere, Category = ItemInfo, meta = (AllowPrivateAccess = "true"))
		EItemType AdditionalItemType = EItemType::NONE;
	UPROPERTY(EditAnywhere, Category = ItemInfo, meta = (AllowPrivateAccess = "true", UIMin = "0"))
		int32 AdditionalGetCount = 0;

	UPROPERTY(EditAnywhere, Category = FirstGetInfo, meta = (AllowPrivateAccess = "true"))
		bool bFirstGetNeedsUpdateUI;
	UPROPERTY(EditAnywhere, Category = FirstGetInfo, meta = (AllowPrivateAccess = "true"))
		FString ItemNameStr;
	UPROPERTY(EditAnywhere, Category = FirstGetInfo, meta = (AllowPrivateAccess = "true"))
		FString ItemDescStr;

};

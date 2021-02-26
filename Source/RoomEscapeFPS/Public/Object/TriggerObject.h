// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/ClueObject.h"
#include "TriggerObject.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnTriggerEvent, APawn*, requester);

UCLASS()
class ROOMESCAPEFPS_API ATriggerObject : public AClueObject
{
	GENERATED_BODY()
	
public:
	ATriggerObject();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual bool OnInteraction(APawn* requester, class UPrimitiveComponent* InComp) override;

	UFUNCTION(Server, Reliable)
		void ServerActivateGhostSpawner(APawn* requester);
	UFUNCTION(Server, Reliable)
		void ServerActivatePipeGame(APawn* requester);
	UFUNCTION(Server, Reliable)
		void ServerCompleteGameAndGotoMainMenu(APawn* requester);

protected:
	FOnTriggerEvent OnTriggerEvent;

	UPROPERTY(EditAnywhere)
		FString DelegateName;
	UPROPERTY(Replicated)
		bool bIsTriggered;

};

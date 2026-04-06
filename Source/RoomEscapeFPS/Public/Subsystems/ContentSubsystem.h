// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface/EventDispatcher/EventSendRecvInterface.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ContentSubsystem.generated.h"

/**
 * 컨텐츠 관리용 서브시스템 베이스 클래스.
 * IEventDispatchRecver를 통해 이벤트를 수신할 수 있는 GameInstanceSubsystem.
 */
UCLASS(Abstract)
class ROOMESCAPEFPS_API UContentSubsystem
	: public UGameInstanceSubsystem
	, public IEventDispatchRecver
{
	GENERATED_BODY()

public:
	void Initialize(FSubsystemCollectionBase& Collection) override;
	void Deinitialize() override;

	void SubscribeEvent() override {};
};

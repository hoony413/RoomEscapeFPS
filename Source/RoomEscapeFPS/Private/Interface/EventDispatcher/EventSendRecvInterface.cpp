// Fill out your copyright notice in the Description page of Project Settings.
#include "Interface/EventDispatcher/EventSendRecvInterface.h"

void IEventDispatchRecver::UnsubscribeAllEvent()
{
	auto* const castedThis = Cast<UObject>(this);
	if (auto* const gameInstance = UGameplayStatics::GetGameInstance(castedThis))
	{
		if (auto* const eventSubsystem = gameInstance->GetSubsystem<UEventDispatcherSubsystem>())
		{
			eventSubsystem->UnbindAll(_boundEvents, castedThis);
			_boundEvents.Empty();
		}
	}
}

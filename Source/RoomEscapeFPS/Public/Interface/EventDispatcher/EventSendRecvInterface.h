#pragma once

#include "Subsystems/EventDispatcherSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "EventSendRecvInterface.generated.h"

class UEventDispatcherSubsystem;

class UContentSubsystem;
class UBaseActivatableWidget;

enum class EEventType : uint8;

UINTERFACE()
class ROOMESCAPEFPS_API UEventDispatchRecver : public UInterface
{
	GENERATED_BODY()
};

class IEventDispatchRecver
{
	GENERATED_BODY()

protected:
	virtual void SubscribeEvent() = 0;

	template<EEventType EventType, typename UserClass, typename... Args>
	void BindEvent(void(UserClass::*Func)(Args...))
	{
		if (auto* const gameInstance = UGameplayStatics::GetGameInstance(Cast<UObject>(this)))
		{
			if (auto* const eventSubsystem = gameInstance->GetSubsystem<UEventDispatcherSubsystem>())
			{
				auto* const userClass = Cast<UserClass>(this);
				eventSubsystem->Bind<EventType>(userClass, Func);
				_boundEvents.AddUnique(EventType);
			}
		}
	}

	void UnsubscribeAllEvent();

private:
	// 오직 UContentSubsystem에게만 인터페이스를 제공하도록 강제.
	friend class UContentSubsystem;
	IEventDispatchRecver() {}

private:
	TArray<EEventType> _boundEvents;
};

UINTERFACE()
class ROOMESCAPEFPS_API UEventDispatchSender : public UInterface
{
	GENERATED_BODY()
};

class IEventDispatchSender
{
	GENERATED_BODY()

protected:
	template<EEventType EventType, typename... Args>
	void Dispatch(Args&&... args)
	{
		if (auto* const gameInstance = UGameplayStatics::GetGameInstance(Cast<UObject>(this)))
		{
			if (auto* const eventSubsystem = gameInstance->GetSubsystem<UEventDispatcherSubsystem>())
			{
				eventSubsystem->Dispatch<EventType>(Forward<Args>(args)...);
			}
		}
	}

private:
	// 오직 UBaseActivatableWidget에게만 인터페이스를 제공하도록 강제.
	friend class UBaseActivatableWidget;
	IEventDispatchSender() {}
};

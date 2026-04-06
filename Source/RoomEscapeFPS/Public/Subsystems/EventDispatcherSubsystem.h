// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utilities/EventDefines.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EventDispatcherSubsystem.generated.h"

UCLASS()
class ROOMESCAPEFPS_API UEventDispatcherSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	void Initialize(FSubsystemCollectionBase& collection) override;
	void Deinitialize() override;
	bool ShouldCreateSubsystem(UObject* outer) const override;

private:
	template<EEventType EventType, typename UserClass, typename... Args>
	void Bind(UserClass* object, void(UserClass::*func)(Args...))
	{
		// 1. derived check
		static_assert(std::is_base_of_v<IEventDispatchRecver, UserClass>, "should inheritance IEventDispatchRecver.");

		// 2. param check
		using ExpectedFuncType = typename TEventTraits<EventType>::FuncType;
		static_assert(std::is_same_v<ExpectedFuncType, void(Args...)>, "parameter mismatch");

		if (not object) return;

		// 3. 중복 바인딩 체크 및 파괴된 객체의 죽은 핸들러 청소(GC)
		auto& handlerList = _listeners.FindOrAdd(EventType);
		handlerList.RemoveAll([](TSharedPtr<FEventHandlerBase> const& handler)
		{
			return not handler.IsValid() || not handler->_boundObject.IsValid();
		});

		for (auto const& handler : handlerList)
		{
			if (handler->_boundObject.Get() == object)
			{
				return;
			}
		}

		// 4. 새로운 핸들러 생성
		using ExpectedHandlerType = TEventHandler<ExpectedFuncType>;
		TSharedPtr<ExpectedHandlerType> newHandler = MakeShared<ExpectedHandlerType>();
		newHandler->_boundObject = object;
		newHandler->_delegate.BindUObject(object, func);

		// 5. 리스너 맵에 추가
		handlerList.Emplace(newHandler);
	}

	void UnbindAll(TArray<EEventType> const& boundEvents, UObject* object)
	{
		if (not object)
		{
			return;
		}

		for (auto const eventType : boundEvents)
		{
			if (TArray<TSharedPtr<FEventHandlerBase>>* eventHandlers = _listeners.Find(eventType))
			{
				eventHandlers->RemoveAll([object](TSharedPtr<FEventHandlerBase> const& handler)
				{
					if (not handler->_boundObject.IsValid() || handler->_boundObject == object)
					{
						handler->Unbind();
						return true;
					}
					return false;
				});
			}
		}
	}

	template<EEventType EventType, typename... Args>
	void Dispatch(Args&&... args)
	{
		if (auto* eventHandlers = _listeners.Find(EventType))
		{
			// 가비지 컬렉션: 이벤트 브로드캐스트 전, 파괴된 객체의 바인딩 흔적들을 안전하게 제거
			eventHandlers->RemoveAll([](TSharedPtr<FEventHandlerBase> const& handlerBase)
			{
				return not handlerBase.IsValid() || not handlerBase->_boundObject.IsValid();
			});

			// 댕글링 방지를 위한 얕은 복사.
			auto const copiedHandlers = *eventHandlers;
			// 1. 모든 리스너에게 이벤트 전달
			for (auto const& handlerBase : copiedHandlers)
			{
				// 2. 저장된 기본 핸들러를 실제 타입의 핸들러로 캐스팅
				using ExpectedHandlerType = TEventHandler<typename TEventTraits<EventType>::FuncType>;
				if (auto const* handler = static_cast<ExpectedHandlerType*>(handlerBase.Get()))
				{
					// 3. 바인딩된 함수 호출
					handler->_delegate.ExecuteIfBound(args...);
				}
			}
		}
	}

	friend class IEventDispatchRecver;
	friend class IEventDispatchSender;

private:
	// 등록된 리스너들을 저장하는 TMap
	TMap<EEventType, TArray<TSharedPtr<FEventHandlerBase>>> _listeners;
};

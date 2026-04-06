// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

struct FEventHandlerBase
{
	virtual ~FEventHandlerBase() = default;

	virtual void Unbind() = 0;
	TWeakObjectPtr<UObject> _boundObject; // 어느 객체에 바인딩되었는지 추적
};

template<typename FuncType>
struct TEventHandler;

// 가변인자 템플릿을 사용하는 실제 핸들러
template<typename... Args>
struct TEventHandler<void(Args...)> : FEventHandlerBase
{
	void Unbind() override
	{
		_delegate.Unbind();
	}

	TDelegate<void(Args...)> _delegate;
};

// 매크로 선언 정의
#define DECLARE_EVENT_TYPE(EventType, ...) EventType,
UENUM(BlueprintType)
enum class EEventType : uint8
{
	NONE = 0 UMETA(Hidden),
	#include "EventTypeValues.inl"
	MAX UMETA(Hidden)
};
#undef DECLARE_EVENT_TYPE

///////////////////// 매크로 재정의
template<EEventType EventType>
struct TEventTraits;
#define DECLARE_EVENT_TYPE(EnumName, ...) \
template<> \
struct TEventTraits<EEventType::EnumName> \
{ \
	using FuncType = void(__VA_ARGS__); \
};
#include "EventTypeValues.inl"
#undef DECLARE_EVENT_TYPE

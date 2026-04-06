// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "Input/UIActionBindingHandle.h"
#include "Interface/EventDispatcher/EventSendRecvInterface.h"
#include "Interface/View/ViewModelInterface.h"
#include "Interface/WidgetAnimationInterface.h"
#include "BaseActivatableWidget.generated.h"

/**
 * CommonUI 기반 위젯 베이스 클래스.
 * IEventDispatchSender를 통해 이벤트를 발송할 수 있다.
 * IViewModelOwnerInterface를 통해 MVVM ViewModel을 연결할 수 있다.
 */
UCLASS(Abstract)
class ROOMESCAPEFPS_API UBaseActivatableWidget
	: public UCommonActivatableWidget
	, public IEventDispatchSender
	, public IWidgetAnimationInterface
	, public IViewModelOwnerInterface
{
	GENERATED_BODY()

public:
	TOptional<FUIInputConfig> GetDesiredInputConfig() const override;

#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:
	void NativeOnInitialized() override;
	void NativeOnActivated() override;
	void NativeOnDeactivated() override;

private:
	void ApplyDesiredInputConfig();
	void RestoreInputConfig();

protected:
	// IViewModelOwnerInterface - 서브클래스에서 오버라이드하여 ViewModel 할당
	void SetViewModel() override {}

	// ViewModel 라이프사이클 훅 - ViewModel이 필요한 서브클래스에서 오버라이드
	virtual void PostSetViewModel() {}
	virtual void ClearViewModel() {}

protected:
	TArray<TObjectPtr<UWidgetAnimation>>& GetAnimationArray() override { return AnimArray; }

protected:
	UPROPERTY(EditAnywhere, Category = "Input")
bool _useCustomInputConfig{};

	UPROPERTY(EditAnywhere, Category = "Input", meta = (EditCondition = "_useCustomInputConfig"))
FUIInputConfig _inputConfig{};

	UPROPERTY(Transient)
TArray<TObjectPtr<UWidgetAnimation>> AnimArray;
};

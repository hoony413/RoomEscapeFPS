#pragma once

#include "Subsystems/ViewModelSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "ViewModelInterface.generated.h"

class UCommonUserWidget;

UINTERFACE()
class ROOMESCAPEFPS_API UViewModelInterface : public UInterface
{
	GENERATED_BODY()
};

class IViewModelInterface
{
	GENERATED_BODY()

public:
	virtual void SubscribeViewModel() = 0;
	virtual void UnsubscribeViewModel() = 0;
};

UINTERFACE()
class ROOMESCAPEFPS_API UViewModelOwnerInterface : public UInterface
{
	GENERATED_BODY()
};

// 위젯에서 사용하는 인터페이스 (View와 ViewModel 연결 담당)
class IViewModelOwnerInterface
{
	GENERATED_BODY()

protected:
	virtual void SetViewModel() = 0;

	template<typename TViewModel>
	requires std::is_base_of_v<IViewModelInterface, TViewModel>
	[[nodiscard]] TViewModel* PrepareViewModel(UCommonUserWidget* widget)
	{
		if (auto* const gameInstance = UGameplayStatics::GetGameInstance(Cast<UObject>(this)))
		{
			if (auto* const vmSubsystem = gameInstance->GetSubsystem<UViewModelSubsystem>())
			{
				return vmSubsystem->MakeViewModel<TViewModel>(widget);
			}
		}

		return nullptr;
	}

private:
	friend class UBaseActivatableWidget;
	friend class UBaseWidget;

	IViewModelOwnerInterface() {}
};

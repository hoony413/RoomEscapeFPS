// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ViewModelSubsystem.generated.h"

UCLASS()
class ROOMESCAPEFPS_API UViewModelSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

private:
	template<typename TViewModel>
	TViewModel* MakeViewModel(UCommonUserWidget* outer)
	{
		if (not outer)
		{
			return nullptr;
		}

		return NewObject<TViewModel>(outer);
	}

	friend class IViewModelOwnerInterface;
};

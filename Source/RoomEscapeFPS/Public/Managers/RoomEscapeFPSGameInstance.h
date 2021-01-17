// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RoomEscapeFPSGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ROOMESCAPEFPS_API URoomEscapeFPSGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	virtual void Shutdown() override;

	template<typename T>
	T* GetManager()
	{
		check(T::StaticClass()->IsChildOf(UBaseManager::StaticClass()));
		if (Managers.Contains(T::StaticClass()->GetFName()))
		{
			return Cast<T>(Managers[T::StaticClass()->GetFName()]);
		}
		// 없다. 만들자
		T* mng = NewObject<T>(this, T::StaticClass()->GetFName());
		mng->InitManager();
		Managers.Add(T::StaticClass()->GetFName(), mng);
		return mng;
	}
private:
	TMap<FName, class UBaseManager*> Managers;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Object/FreelistActor.h"
#include "Freelist.generated.h"

/**
 * 
 */
UCLASS()
class ROOMESCAPEFPS_API UFreelist : public UObject
{
	GENERATED_BODY()
	
public:

	template<typename T>
	T* GetElement()
	{
		T* t = nullptr;
		if (FreeList.Num() <= 0)
		{
			t = NewObject<T>(this);
		}
		else
		{
			t = FreeList.GetHead()->GetValue();
		}

		t->SetIsInFreeList(false);
		return t;
	}

	template<typename T>
	void ReturnElement(T* t)
	{
		t->SetIsInFreeList(true);
		if (FreeList.Num() <= 0)
		{
			FreeList.AddHead(t);
		}
		else
		{
			FreeList.AddTail(t);
		}
	}

	void ReleaseFreeList()
	{
		for (auto it = FreeList.GetHead(); it != nullptr; ++it)
		{
			AFreelistActor* actor = it->GetValue();
			actor->Destroy();
		}
		FreeList.Empty();
	}

private:
	TDoubleLinkedList<AFreelistActor*> FreeList;
};

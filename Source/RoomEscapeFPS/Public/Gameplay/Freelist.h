// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Helper/Helper.h"
#include "Freelist.generated.h"

/**
 * 
 */
class UFreelistObjectInterface;
class IFreelistObjectInterface;

UCLASS()
class ROOMESCAPEFPS_API UFreelist : public UActorComponent
{
	GENERATED_BODY()
	
public:

	// Sets default values for this actor's properties
	UFreelist();

	template<typename T>
	T* GetElement()
	{
		check(T::StaticClass()->IsChildOf(AActor::StaticClass()));
		T* t = nullptr;

		bool bFind = false;
		int32 i = 0;
		for (; i < FreeList.Num(); ++i)
		{
			if (FreeList[i] == nullptr)
				break;
	
			IFreelistObjectInterface* freelistObj = Cast<IFreelistObjectInterface>(FreeList[i]);
			check(freelistObj);
			if (freelistObj->IsInFreeList())
			{
				t = Cast<T>(FreeList[i]);
				bFind = true;
				break;
			}
		}

		if (bFind == false)
		{
			t = GetWorld()->SpawnActor<T>(TargetObjectToPooling.LoadSynchronous());
			FreeList.Add(t);
		}
		check(t);
		t->SetIsInFreeList(false);
		return t;
	}

	template<typename T>
	void ReturnElement(T* t)
	{
		t->SetIsInFreeList(true);
	}

	void ReleaseFreeList();

	UPROPERTY(EditAnywhere)
		TSoftClassPtr<class AActor> TargetObjectToPooling;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TArray<class AActor*> FreeList;

};

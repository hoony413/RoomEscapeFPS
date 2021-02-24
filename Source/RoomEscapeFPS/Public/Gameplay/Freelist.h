// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Helper/Helper.h"
#include "GameFramework/Pawn.h"
#include "Freelist.generated.h"

/**
 * 
 */
class IFreelistObjectInterface;

UCLASS()
class ROOMESCAPEFPS_API UFreelist : public UActorComponent
{
	GENERATED_BODY()
	
public:

	// Sets default values for this actor's properties
	UFreelist();

	template<typename T>
	T* GetElement(class APawn* InOwner = nullptr)
	{
		check(T::StaticClass()->IsChildOf(AActor::StaticClass()));
		T* t = nullptr;

		bool bFind = false;
		for (int32 i = 0; i < FreeList.Num(); ++i)
		{
			if (FreeList[i] == nullptr)
				break;
	
			IFreelistObjectInterface* freelistObj = Cast<IFreelistObjectInterface>(FreeList[i]);
			check(freelistObj);
			if (freelistObj->IsInFreeList())
			{
				t = Cast<T>(FreeList[i]);
				t->SetOwner(InOwner);
				bFind = true;
				break;
			}
		}

		if (bFind == false)
		{
			FActorSpawnParameters params;
			params.Owner = InOwner;
			params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
			t = GetWorld()->SpawnActor<T>(TargetObjectToPooling.LoadSynchronous(), params);
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
	virtual void BeginDestroy() override;

private:
	UPROPERTY()
	TArray<class AActor*> FreeList;

};

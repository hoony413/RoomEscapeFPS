// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/SwitchButtonWrapper.h"
#include "Object/SwitchButtonObject.h"

// Called when the game starts or when spawned
void ASwitchButtonWrapper::BeginPlay()
{
	Super::BeginPlay();
	for (int32 i = 0, j = ChildActors.Num() - 1; i < ChildActors.Num(); ++i, --j)
	{
		ASwitchButtonObject* obj = Cast<ASwitchButtonObject>(ChildActors[i]->GetChildActor());
		if (obj)
		{
			if (obj->OnInteractionHappened.IsBound() == false)
			{
				obj->OnInteractionHappened.BindUObject(this, &ASwitchButtonWrapper::ServerOnStateChanged);
			}
			obj->SetDigit(j);
		}
	}
}
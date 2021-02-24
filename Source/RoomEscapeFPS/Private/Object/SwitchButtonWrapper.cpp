// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/SwitchButtonWrapper.h"

// Called when the game starts or when spawned
void ASwitchButtonWrapper::BeginPlay()
{
	Super::BeginPlay();
	for (int32 i = 0, j = ChildActors.Num() - 1; i < ChildActors.Num(); ++i, --j)
	{
		ASwitchButtonObject* obj = Cast<ASwitchButtonObject>(ChildActors[i]);
		if (obj)
		{
			if (obj->OnChildObjectChanged.IsBound() == false)
			{
				obj->OnChildObjectChanged.BindUObject(this, &ASwitchButtonWrapper::ServerOnStateChanged);
			}
			obj->SetDigit(j);
		}
	}
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/PaintingWrapper.h"
#include "Object/PaintingObject.h"

// Called when the game starts or when spawned
void APaintingWrapper::BeginPlay()
{
	Super::BeginPlay();
	for (int32 i = 0, j = ChildActors.Num() - 1; i < ChildActors.Num(); ++i, --j)
	{
		APaintingObject* obj = Cast<APaintingObject>(ChildActors[i]);
		if (obj)
		{
			if (obj->OnChildObjectChanged.IsBound() == false)
			{
				obj->OnChildObjectChanged.BindUObject(this, &APaintingWrapper::ServerOnStateChanged);
			}
		}
	}
}
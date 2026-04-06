// Fill out your copyright notice in the Description page of Project Settings.
#include "Subsystems/EventDispatcherSubsystem.h"

void UEventDispatcherSubsystem::Initialize(FSubsystemCollectionBase& collection)
{
	Super::Initialize(collection);
}

void UEventDispatcherSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

bool UEventDispatcherSubsystem::ShouldCreateSubsystem(UObject* outer) const
{
	return true;
}

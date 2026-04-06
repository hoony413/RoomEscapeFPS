// Fill out your copyright notice in the Description page of Project Settings.
#include "Subsystems/ContentSubsystem.h"

void UContentSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	Collection.InitializeDependency(UEventDispatcherSubsystem::StaticClass());

	SubscribeEvent();
}

void UContentSubsystem::Deinitialize()
{
	UnsubscribeAllEvent();
	Super::Deinitialize();
}

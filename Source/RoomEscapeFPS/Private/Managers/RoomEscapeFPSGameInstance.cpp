// Fill out your copyright notice in the Description page of Project Settings.

#include "Managers/RoomEscapeFPSGameInstance.h"
#include "Managers/BaseManager.h"

void URoomEscapeFPSGameInstance::Init()
{
	Super::Init();
	for (const auto& elem : Managers)
	{
		elem.Value->InitManager();
	}
}
void URoomEscapeFPSGameInstance::Shutdown()
{
	for (const auto& elem : Managers)
	{
		elem.Value->ReleaseManager();
	}
	Managers.Empty();
	Super::Shutdown();
}
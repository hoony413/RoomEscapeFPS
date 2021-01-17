// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/BaseManager.h"

void UBaseManager::InitManager()
{
	if (IsRooted() == false)
		AddToRoot();
}
void UBaseManager::ReleaseManager()
{
	RemoveFromRoot();
}

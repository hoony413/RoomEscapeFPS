// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/RoomEscapeFPSGameState.h"
#include "Object/InteractiveObject.h"
#include "GameFrameWork/RoomEscapeFPSPlayerState.h"

void ARoomEscapeFPSGameState::BeginPlay()
{
	Super::BeginPlay();
}
void ARoomEscapeFPSGameState::AddToSolutionResultActor(class AInteractiveObject* InObj)
{

}
void ARoomEscapeFPSGameState::OnCorrectAnswer(EServerSolutionType InType)
{
	AInteractiveObject* obj = FindResultActor(InType);
	if (obj)
	{	
		// TODO: ����� ��ϵ� ��������Ʈ ����.
		//obj->
	}
}

AInteractiveObject* ARoomEscapeFPSGameState::FindResultActor(EServerSolutionType InType)
{
	for (auto& elem : OnCorrectResultActor)
	{
		// TODO: ������Ʈ�� ���� Ÿ���� üũ�Ͽ� ����.
	}

	return nullptr;
}
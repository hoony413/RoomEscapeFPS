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
		// TODO: 결과로 등록된 델리게이트 실행.
		//obj->
	}
}

AInteractiveObject* ARoomEscapeFPSGameState::FindResultActor(EServerSolutionType InType)
{
	for (auto& elem : OnCorrectResultActor)
	{
		// TODO: 오브젝트의 현재 타입을 체크하여 리턴.
	}

	return nullptr;
}
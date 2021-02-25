// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/RoomEscapeFPSGameState.h"
#include "GameFrameWork/RoomEscapeFPSPlayerState.h"
#include "Object/InteractiveObject.h"
#include "Helper/Helper.h"
#include "Managers/UIManager.h"
#include "UI/NoticePanel.h"

void ARoomEscapeFPSGameState::BeginPlay()
{
	Super::BeginPlay();
}
void ARoomEscapeFPSGameState::AddToSolutionResultObject(class AInteractiveObject* InObj)
{
	OnSolutionResultObject.Add(InObj);
}
void ARoomEscapeFPSGameState::OnCorrectAnswer(EServerSolutionType InType)
{
	AInteractiveObject* obj = FindResultActor(Helper::GetSolutionResultType(InType));
	if (obj)
	{	// TODO: ����� ��ϵ� ��������Ʈ ����.
		if (obj->OnSolutionSuccessResult.IsBound())
		{
			obj->OnSolutionSuccessResult.Execute(nullptr, obj->GetSolutionResultComp());
		}
	}
	NetMulticastOpenDoorNotice();
}
void ARoomEscapeFPSGameState::NetMulticastOpenDoorNotice_Implementation()
{
	if (GetNetMode() == NM_Client)
	{
		UNoticePanel* notice = GetUIMgr()->GetWidget<UNoticePanel>();
		if (notice)
		{
			notice->OpenNotice(ENoticeType::EOpenNextDoor);
		}
	}
}
AInteractiveObject* ARoomEscapeFPSGameState::FindResultActor(EServerSolutionResultType InType)
{
	for (auto& elem : OnSolutionResultObject)
	{
		// TODO: ������Ʈ�� ���� Ÿ���� üũ�Ͽ� ����.
		if (InType == elem->GetSolutionResultType())
		{
			return elem;
		}
	}
	return nullptr;
}
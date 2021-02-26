// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/RoomEscapeFPSGameState.h"
#include "GameFramework/RoomEscapeFPSGameMode.h"
#include "GameFramework/RoomEscapeFPSPlayerState.h"
#include "GameFramework/RoomEscapeFPSPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Object/InteractiveObject.h"
#include "Gameplay/GhostSpawner.h"
#include "Helper/Helper.h"
#include "Managers/UIManager.h"
#include "UI/NoticePanel.h"
#include "Net/UnrealNetwork.h"

void ARoomEscapeFPSGameState::BeginPlay()
{
	Super::BeginPlay();
}
void ARoomEscapeFPSGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ARoomEscapeFPSGameState, GhostDeadCount);
}
void ARoomEscapeFPSGameState::ServerIncreaseGhostDeadCount_Implementation()
{
	if (GetNetMode() == NM_DedicatedServer)
	{
		++GhostDeadCount;
		ARoomEscapeFPSGameMode* gm = Helper::GetGameMode(GetWorld());
		if (gm)
		{
			if (gm->CheckAnswer(GhostDeadCount, EServerSolutionType::EGhostDeadCount_Target))
			{
				gm->SetActiveGhostSpawner(false);
				OnCorrectAnswer(EServerSolutionType::EGhostDeadCount_Target);
				Helper::UpdateNextUIInfo(GetWorld(), ENextInformationType::ECaptureGhost, ENextInformationType::EPipelineComplete, GhostDeadCount);
			}
			else
			{
				Helper::UpdateNextUIInfo(GetWorld(), ENextInformationType::ECaptureGhost, ENextInformationType::ECaptureGhost, GhostDeadCount);
			}
		}
	}
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

	if (InType != EServerSolutionType::EPipelineGame_Complete)
	{
		// ���������� ������ ������ ���� ���� ������ �˾��� �����ǹǷ� ���⼭ �� �ʿ䰡 ����.
		NetMulticastOpenDoorNotice();
	}
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

void ARoomEscapeFPSGameState::SetActiveGhostSpawner(bool bActive)
{
	auto FindGhostSpawner = [&](AGhostSpawner* spawner) -> bool
	{
		if(spawner != nullptr)
			return true;
		return false;
	};
	AGhostSpawner* spn = Helper::FindActor<AGhostSpawner>(GetWorld(), FindGhostSpawner);
	if (spn)
	{
		spn->SetActive(bActive);
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
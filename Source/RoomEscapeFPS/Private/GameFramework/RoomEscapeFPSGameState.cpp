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
	{	// TODO: 결과로 등록된 델리게이트 실행.
		if (obj->OnSolutionSuccessResult.IsBound())
		{
			obj->OnSolutionSuccessResult.Execute(nullptr, obj->GetSolutionResultComp());
		}
	}

	if (InType != EServerSolutionType::EPipelineGame_Complete)
	{
		// 파이프라인 게임은 별도의 연출 이후 별도로 팝업이 생성되므로 여기서 할 필요가 없다.
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
		// TODO: 오브젝트의 현재 타입을 체크하여 리턴.
		if (InType == elem->GetSolutionResultType())
		{
			return elem;
		}
	}
	return nullptr;
}
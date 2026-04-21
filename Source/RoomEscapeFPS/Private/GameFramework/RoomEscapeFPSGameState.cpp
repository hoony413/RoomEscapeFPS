// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/RoomEscapeFPSGameState.h"
#include "GameFramework/RoomEscapeFPSGameMode.h"
#include "Object/InteractiveObject.h"
#include "Gameplay/GhostSpawner.h"
#include "Helper/Helper.h"
#include "Managers/UISubsystem.h"
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
void ARoomEscapeFPSGameState::IncreaseGhostDeadCount()
{
	if (IsNetMode(NM_DedicatedServer))
	{
		++GhostDeadCount;
		ARoomEscapeFPSGameMode* gm = Helper::GetGameMode(GetWorld());
		if (gm)
		{
			if (gm->CheckAnswer(GhostDeadCount, EServerSolutionType::GHOST_DEAD_COUNT_TARGET))
			{
				gm->SetActiveGhostSpawner(false);
				OnCorrectAnswer(EServerSolutionType::GHOST_DEAD_COUNT_TARGET);
				Helper::UpdateNextUIInfo(GetWorld(), ENextInformationType::CAPTURE_GHOST, ENextInformationType::PIPELINE_COMPLETE, gm->GetGhostDeadCountTarget());
			}
			else
			{
				Helper::UpdateNextUIInfo(GetWorld(), ENextInformationType::CAPTURE_GHOST, ENextInformationType::CAPTURE_GHOST, GhostDeadCount);
			}
		}
	}
}
void ARoomEscapeFPSGameState::AddToSolutionResultObject(AInteractiveObject* InObj)
{
	OnSolutionResultObject.Add(InObj);
}

void ARoomEscapeFPSGameState::OnCorrectAnswer(EServerSolutionType InType)
{
	AInteractiveObject* obj = FindResultActor(Helper::GetSolutionResultType(InType));
	if (obj)
	{
		if (obj->OnSolutionSuccessResult.IsBound())
		{
			obj->OnSolutionSuccessResult.Execute(nullptr, obj->GetSolutionResultComp());
		}
	}

	if (InType != EServerSolutionType::PIPELINE_GAME_COMPLETE)
	{
		// 파이프라인 게임은 별도의 연출 이후 별도로 팝업이 생성되므로 여기서 할 필요가 없다.
		NetMulticastOpenDoorNotice();
	}
}

void ARoomEscapeFPSGameState::NetMulticastOpenDoorNotice_Implementation()
{
	if (not IsNetMode(NM_Client))
	{
		return;
	}
	
	UUISubsystem* uiSubsystem = Helper::GetSubsystem<UUISubsystem>(GetWorld());
	if (not uiSubsystem)
	{
		return;
	}
	
	if (UNoticePanel* notice = uiSubsystem->OpenWidget<UNoticePanel>(EActivatableWidgetType::NOTICE))
	{
		notice->OpenNotice(ENoticeType::OPEN_NEXT_DOOR);
	}
}

void ARoomEscapeFPSGameState::SetActiveGhostSpawner(bool bActive)
{
	auto FindGhostSpawner = [&](AGhostSpawner* spawner) -> bool
	{
		if (IsValid(spawner))
		{
			return true;
		}
		return false;
	};
	
	if (AGhostSpawner* spn = Helper::FindActor<AGhostSpawner>(GetWorld(), FindGhostSpawner))
	{
		spn->SetActive(bActive);
	}
}

AInteractiveObject* ARoomEscapeFPSGameState::FindResultActor(EServerSolutionResultType InType)
{
	for (auto const& elem : OnSolutionResultObject)
	{
		if (InType == elem->GetSolutionResultType())
		{
			return elem;
		}
	}
	return nullptr;
}
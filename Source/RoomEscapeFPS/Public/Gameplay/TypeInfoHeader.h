// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 여러 클래스나 구조체에서 쓰일 타입 정의.
 */

UENUM()
enum class EItemType : uint8
{
	NONE = 0u,
	Flash,
	Charm,
	Key,
	Battery,
	BatteryPower,
	Book,
	Diary,
	GhostNPC,
};

UENUM()
enum class EServerSolutionType : uint8
{
	ENONE,
	ESolution_1,
	ESolution_2,
	EGhostDeadCount_Target,
	EPipelineGame_Complete,
};

UENUM()
enum class EServerSolutionResultType : uint8
{
	ENONE,
	ESolutionResult_1,
	ESolutionResult_2,
	ESolutionResult_3,
	ESolutionResult_4,
};

UENUM()
enum class ENextInformationType : uint8
{
	EFindLantern,		// interactive object
	ESolveClue_1,		// solution wrapper
	ESolveClue_2,		// solution wrapper
	ECaptureGhost,		// ghostsoul
	EPipelineComplete,  // playerstate
	ERunaway,			// playerstate
};
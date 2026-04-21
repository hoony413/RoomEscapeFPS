// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 여러 클래스나 구조체에서 쓰일 타입 정의.
 */

UENUM()
enum class EItemType : uint8
{
	NONE = 0 UMETA(Hidden),
	FLASH,
	CHARM,
	KEY,
	BATTERY,
	BATTERY_POWER,
	BOOK,
	DIARY,
	GHOST_NPC,
	MAX UMETA(Hidden)
};

UENUM()
enum class EServerSolutionType : uint8
{
	NONE = 0 UMETA(Hidden),
	SOLUTION_1,
	SOLUTION_2,
	GHOST_DEAD_COUNT_TARGET,
	PIPELINE_GAME_COMPLETE,
	MAX UMETA(Hidden)
};

UENUM()
enum class EServerSolutionResultType : uint8
{
	NONE = 0 UMETA(Hidden),
	SOLUTION_RESULT_1,
	SOLUTION_RESULT_2,
	SOLUTION_RESULT_3,
	SOLUTION_RESULT_4,
	MAX UMETA(Hidden)
};

UENUM()
enum class ENextInformationType : uint8
{
	FIND_LANTERN,		// interactive object
	SOLVE_CLUE_1,		// solution wrapper
	SOLVE_CLUE_2,		// solution wrapper
	CAPTURE_GHOST,		// ghostsoul
	PIPELINE_COMPLETE,	// playerstate
	RUNAWAY,			// playerstate
	MAX UMETA(Hidden)
};
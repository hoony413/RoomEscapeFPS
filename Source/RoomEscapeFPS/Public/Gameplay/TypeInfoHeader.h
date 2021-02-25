// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * ���� Ŭ������ ����ü���� ���� Ÿ�� ����.
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
};

UENUM()
enum class EServerSolutionType : uint8
{
	ENONE,
	ESolution_1,
	ESolution_2,
};

UENUM()
enum class EServerSolutionResultType : uint8
{
	ENONE,
	ESolutionResult_1,
	ESolutionResult_2,
};

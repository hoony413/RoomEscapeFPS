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
};

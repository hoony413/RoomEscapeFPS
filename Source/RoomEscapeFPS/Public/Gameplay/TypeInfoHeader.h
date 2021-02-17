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
};

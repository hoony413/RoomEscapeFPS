// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FreelistObjectInterface.generated.h"

/**
 * Freelist 오브젝트를 위한 인터페이스 클래스
 */

UINTERFACE()
class ROOMESCAPEFPS_API UFreelistObjectInterface : public UInterface
{
	GENERATED_BODY()
};

class IFreelistObjectInterface
{
	GENERATED_BODY()

public:
	virtual bool IsInFreeList() = 0;
	virtual void SetIsInFreeList(bool bFreeList) = 0;

};
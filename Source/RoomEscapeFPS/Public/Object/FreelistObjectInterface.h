// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FreelistObjectInterface.generated.h"

/**
 * Freelist ������Ʈ�� ���� �������̽� Ŭ����
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
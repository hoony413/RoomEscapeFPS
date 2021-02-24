// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Gameplay/TypeInfoHeader.h"
#include "RoomEscapeFPSGameState.generated.h"

/**
 * GameState: �÷��̾ �����ϸ� GameState�� �ν��Ͻ��Ѵ�.
 GameState�� Ŭ���̾�Ʈ/���� ��� ���� �����ϸ�, ���� ������ ���� ����(���״�� GameState)
 �� ������Ʈ�ϴ� ������ ���� Ŭ�����̴�.
 GameState�� ���� �����Ͽ� �� �÷��̾�(Client)�� ������ ���ӻ��¿� �°� ������ ������ ��
 �ֵ��� ���´�.
 */

UCLASS()
class ROOMESCAPEFPS_API ARoomEscapeFPSGameState : public AGameStateBase
{
	GENERATED_BODY()

		
public:
	void OnCorrectAnswer(EServerSolutionType InType);

	void AddToSolutionResultActor(class AInteractiveObject* InObj);

protected:
	virtual void BeginPlay() override;
	class AInteractiveObject* FindResultActor(EServerSolutionType InType);

protected:
	UPROPERTY()
	TArray<class AInteractiveObject*> OnCorrectResultActor;
};

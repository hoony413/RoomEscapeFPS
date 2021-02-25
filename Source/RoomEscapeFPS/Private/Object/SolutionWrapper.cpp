// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/SolutionWrapper.h"
#include "Object/SwitchButtonObject.h"
#include "Object/PaintingObject.h"
#include "GameFramework/RoomEscapeFPSGameMode.h"
#include "GameFramework/RoomEscapeFPSGameState.h"
#include "Components/ChildActorComponent.h"

// Sets default values
ASolutionWrapper::ASolutionWrapper()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ParentComp = CreateDefaultSubobject<USceneComponent>(TEXT("Parent"));
	ParentComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASolutionWrapper::BeginPlay()
{
	Super::BeginPlay();
	
	// 자식액터들 확보 후 컴포넌트 태그 번호 오름차순 정렬.
	GetComponents<UChildActorComponent>(ChildActors);
	Algo::Sort(ChildActors, [&](const UChildActorComponent* lhs, const UChildActorComponent* rhs)
	{
		int32 left = FCString::Atoi(*lhs->ComponentTags[0].ToString());
		int32 right = FCString::Atoi(*rhs->ComponentTags[0].ToString());
		return left < right; 
	});
}

void ASolutionWrapper::ServerOnStateChanged_Implementation()
{
	if (GetNetMode() == NM_DedicatedServer)
	{
		ARoomEscapeFPSGameMode* gm = Cast<ARoomEscapeFPSGameMode>(GetWorld()->GetAuthGameMode());
		if (gm)
		{
			int32 answer = 0;
			if (SolutionType == EServerSolutionType::ESolution_1)
			{
				for (int32 i = 0; i < ChildActors.Num(); ++i)
				{
					ASwitchButtonObject* obj = Cast<ASwitchButtonObject>(ChildActors[i]->GetChildActor());
					if (obj && obj->GetIsPressed())
					{
						int32 shift = obj->GetDigit();
						answer += 1 << shift;
					}
				}
			}
			else if (SolutionType == EServerSolutionType::ESolution_2)
			{
				for (int32 i = 0; i < ChildActors.Num(); ++i)
				{
					APaintingObject* obj = Cast<APaintingObject>(ChildActors[i]->GetChildActor());
					if (obj)
					{
						int32 digit = obj->GetDigit();
						answer += (int32)FMath::Pow(10, digit) * (uint8)obj->GetRotateState();
					}
				}
			}

			bool bSuccess = gm->CheckAnswer(answer, SolutionType);
			if (bSuccess)
			{
				// TODO: GameState에서 문제 풀이 성공에 따른 결과 로직 수행.
				gm->GetGameState<ARoomEscapeFPSGameState>()->OnCorrectAnswer(SolutionType);
			}
		}
	}
}
// Called every frame
void ASolutionWrapper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


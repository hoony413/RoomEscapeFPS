// Fill out your copyright notice in the Description page of Project Settings.

#include "Object/InteractiveObject.h"
#include "Engine/Classes/Components/BoxComponent.h"
#include "DrawDebugHelpers.h"
#include "Actor.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFrameWork/Character.h"
#include "Components/StaticMeshComponent.h"
#include "UI/InteractionPanel.h"
#include "Helper/Helper.h"
#include "Managers/UIManager.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AInteractiveObject::AInteractiveObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	TimelineMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TimelineMesh"));
	TimelineMesh->SetOnlyOwnerSee(false);
	TimelineMesh->SetupAttachment(RootComponent);

	LineTraceBox = CreateDefaultSubobject<UBoxComponent>(TEXT("LineTraceBox"));
	LineTraceBox->SetupAttachment(TimelineMesh);

	if (HasAuthority())
	{
		CurrentState = EInteractiveObjectState::EState_Close_Or_Off;
	}
}
void AInteractiveObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AInteractiveObject, CurrentState);
}
// Called when the game starts or when spawned
void AInteractiveObject::BeginPlay()
{
	Super::BeginPlay();
	
	LineTraceBox->SetBoxExtent(LineTraceBoxSize);
	LineTraceBox->SetRelativeLocation(LineTraceBoxOffset);

	if (IsUseTimeline)
	{
		SetTimeline();
	}

	//DEBUG_BOX_BLUE(LineTraceBox, GetActorLocation() + LineTraceBox->GetRelativeLocation());
}

void AInteractiveObject::SetTimeline()
{
	if (TimelineCurve != nullptr)
	{
		FOnTimelineFloat TimelineCallback;
		TimelineCallback.BindUFunction(this, FName("LaunchTimeline"));
		Timeline.AddInterpFloat(TimelineCurve, TimelineCallback);

		//FOnTimelineEventStatic TimelineFinishedCallback;
		//TimelineFinishedCallback.BindUFunction(this, FName{ TEXT("") });
		//Timeline.SetTimelineFinishedFunc()
	}
}

void AInteractiveObject::LaunchTimelineInternal()
{
	if (TimelineCurve)
	{
		TimelineDelta = Timeline.GetPlaybackPosition();
		CurveFloatValue = CurveWeightValue * TimelineCurve->GetFloatValue(TimelineDelta);

		FQuat NewRotation = FQuat(FRotator(0.f, CurveFloatValue, 0.f));
		TimelineMesh->SetRelativeRotation(NewRotation);
	}
}
// Called every frame
void AInteractiveObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsUseTimeline)
	{
		Timeline.TickTimeline(DeltaTime);
	}
}
EInteractiveObjectState AInteractiveObject::GetNextState()
{
	if (CurrentState == EInteractiveObjectState::EState_Open_Or_On)
	{
		return EInteractiveObjectState::EState_Close_Or_Off;
	}
	else if (CurrentState == EInteractiveObjectState::EState_Close_Or_Off)
	{
		return EInteractiveObjectState::EState_Open_Or_On;
	}
	return EInteractiveObjectState::EState_Playing;
}
void AInteractiveObject::OnInteraction(EInteractiveObjectState NextState)
{
	NetMulticast_Interaction(NextState);
}
void AInteractiveObject::NetMulticast_Interaction_Implementation(EInteractiveObjectState NextState)
{
	if (IsUseTimeline)
	{	// 타임라인 처리.
		if (CurrentState == EInteractiveObjectState::EState_Playing)
		{
			return;
		}
		else if (NextState == EInteractiveObjectState::EState_Close_Or_Off)
		{
			Timeline.ReverseFromEnd();
		}
		else if (NextState == EInteractiveObjectState::EState_Open_Or_On)
		{
			Timeline.PlayFromStart();
		}
	}
}
void AInteractiveObject::ToggleState()
{
	if (HasAuthority())
	{
		if (CurrentState == EInteractiveObjectState::EState_Open_Or_On)
		{
			CurrentState = EInteractiveObjectState::EState_Close_Or_Off;
		}
		else if (CurrentState == EInteractiveObjectState::EState_Close_Or_Off)
		{
			CurrentState = EInteractiveObjectState::EState_Open_Or_On;
		}
		else
		{
			CurrentState = EInteractiveObjectState::EState_Playing;
		}
	}
}
void AInteractiveObject::OnRep_CurrentState()
{

}
#if WITH_EDITOR
void AInteractiveObject::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (PropertyChangedEvent.MemberProperty)
	{
		FName propertyName = PropertyChangedEvent.MemberProperty->GetFName();
		if (propertyName == GET_MEMBER_NAME_CHECKED(AInteractiveObject, LineTraceBoxSize))
		{
			LineTraceBox->SetBoxExtent(LineTraceBoxSize);
		}
		else if (propertyName == GET_MEMBER_NAME_CHECKED(AInteractiveObject, LineTraceBoxOffset))
		{
			LineTraceBox->SetRelativeLocation(LineTraceBoxOffset);
		}
	}
}
#endif
// Fill out your copyright notice in the Description page of Project Settings.

#include "Object/InteractiveObject.h"
#include "Components/BoxComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/RoomEscapeFPSGameState.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AInteractiveObject::AInteractiveObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	ParentComp = CreateDefaultSubobject<USceneComponent>(TEXT("Parent"));
	ParentComp->SetupAttachment(RootComponent);

	DefaultMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DefaultMesh"));
	DefaultMesh->SetOnlyOwnerSee(false);
	DefaultMesh->SetupAttachment(ParentComp);

	LineTraceBox = CreateDefaultSubobject<UBoxComponent>(TEXT("LineTraceBox"));
	LineTraceBox->SetupAttachment(ParentComp);

	LineTraceBox->SetCollisionProfileName(FName(TEXT("Interaction")));

	TimelineMeshes.Empty();
	//InformationStr = TEXT("Press 'E' key to use");

	bIsNonInteractable = false;
	SolutionResultType = EServerSolutionResultType::NONE;
}

void AInteractiveObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AInteractiveObject, TimelineMeshes);
	DOREPLIFETIME_CONDITION(AInteractiveObject, bIsNonInteractable, COND_InitialOnly);
	DOREPLIFETIME(AInteractiveObject, SolutionResultType);
}

// Tags
const FName TimelineMeshTag = FName(TEXT("TimelineMesh"));

// Called when the game starts or when spawned
void AInteractiveObject::BeginPlay()
{
	Super::BeginPlay();
	
	LineTraceBox->SetBoxExtent(LineTraceBoxSize);
	LineTraceBox->SetRelativeLocation(LineTraceBoxOffset);

	TArray<UStaticMeshComponent*> meshes;
	GetComponents<UStaticMeshComponent>(meshes);
	for (int32 i = 0, j = 0; i < meshes.Num(); ++i)
	{
		if (IsUseTimeline && meshes[i]->ComponentHasTag(TimelineMeshTag))
		{
			TimelineMeshes[j].StaticMeshComponent = meshes[i];
			TimelineMeshes[j].StaticMeshComponent->SetCollisionProfileName(FName(TEXT("ServerInteraction")));

			if (cachedSolutionResultComp == nullptr)
			{
				cachedSolutionResultComp = Cast<UPrimitiveComponent>(TimelineMeshes[j].StaticMeshComponent);
			}
			++j;
		}
	}
	
	SetTimeline();

	if (EServerSolutionResultType::NONE != SolutionResultType && IsNetMode(NM_DedicatedServer))
	{
		OnSolutionSuccessResult.BindUObject(this, &AInteractiveObject::OnInteraction);
		ARoomEscapeFPSGameState* gs = Cast<ARoomEscapeFPSGameState>(GetWorld()->GetGameState());
		if (gs)
		{
			gs->AddToSolutionResultObject(this);
		}
	}

#if WITH_EDITOR
	//DEBUG_BOX_BLUE(LineTraceBox, GetActorLocation() + LineTraceBox->GetRelativeLocation());
#endif
}

void AInteractiveObject::SetTimeline()
{
	if (TimelineCurve != nullptr)
	{
		for (auto& elem : TimelineMeshes)
		{
			auto Func = [this, &elem](float fNotUseInLambda)
			{
				if (TimelineCurve)
				{
					TimelineDelta = elem.Timeline.GetPlaybackPosition();
					CurveFloatValue = elem.fCurveWeightValue * TimelineCurve->GetFloatValue(TimelineDelta);
					switch (elem.ControlType)
					{
					case ETimelineControlType::LOCATION_X:
					{
						FVector NewLocation(elem.StaticMeshComponent->GetRelativeLocation());
						NewLocation.X = StartCurveValue + CurveFloatValue;
						elem.StaticMeshComponent->SetRelativeLocation(NewLocation);
					}
					break;
					case ETimelineControlType::LOCATION_Y:
					{
						FVector NewLocation(elem.StaticMeshComponent->GetRelativeLocation());
						NewLocation.Y = StartCurveValue + CurveFloatValue;
						elem.StaticMeshComponent->SetRelativeLocation(NewLocation);
					}
					break;
					case ETimelineControlType::LOCATION_Z:
					{
						FVector NewLocation(elem.StaticMeshComponent->GetRelativeLocation());
						NewLocation.Z = StartCurveValue + CurveFloatValue;
						elem.StaticMeshComponent->SetRelativeLocation(NewLocation);
					}
					break;
					case ETimelineControlType::ROTATION_X: // Roll
					{
						//FRotator NewRotation(elem.StaticMeshComponent->GetRelativeRotation());
						//NewRotation.Roll = StartCurveValue + CurveFloatValue;
						//elem.StaticMeshComponent->SetRelativeRotation(NewRotation);

						FRotator NewRotation(0.f, 0.f, StartCurveValue + CurveFloatValue);
						elem.StaticMeshComponent->SetRelativeRotation(NewRotation);
					}
					break;
					case ETimelineControlType::ROTATION_Y: // Pitch
					{
						//FRotator NewRotation(elem.StaticMeshComponent->GetRelativeRotation());
						//NewRotation.Pitch = StartCurveValue + CurveFloatValue;
						//elem.StaticMeshComponent->SetRelativeRotation(NewRotation);

						FRotator NewRotation(StartCurveValue + CurveFloatValue, 0.f, 0.f);
						elem.StaticMeshComponent->SetRelativeRotation(NewRotation);
					}
					break;
					case ETimelineControlType::ROTATION_Z: // Yaw
					{
						//FRotator NewRotation(elem.StaticMeshComponent->GetRelativeRotation());
						//NewRotation.Yaw = StartCurveValue + CurveFloatValue;
						//elem.StaticMeshComponent->SetRelativeRotation(NewRotation);

						FRotator NewRotation(0.f, StartCurveValue + CurveFloatValue, 0.f);
						elem.StaticMeshComponent->SetRelativeRotation(NewRotation);
					}
					break;
					}
				}
			};
			
			FOnTimelineFloatStatic TimelineCallback;
			TimelineCallback.BindLambda(Func);
			elem.Timeline.AddInterpFloat(TimelineCurve, TimelineCallback);
		}
	}
}
// Called every frame
void AInteractiveObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsUseTimeline)
	{
		for (auto& elem : TimelineMeshes)
		{
			elem.Timeline.TickTimeline(DeltaTime);
		}
	}
}

FTimelineInfo* AInteractiveObject::FindTimelineMeshComponent(class UStaticMeshComponent* InMesh, int32& OutIndex)
{
	for(int32 i = 0; i < TimelineMeshes.Num(); ++i)
	{
		if (InMesh == TimelineMeshes[i].StaticMeshComponent)
		{
			OutIndex = i;
			return &TimelineMeshes[i];
		}
	}
	return nullptr;
}

bool AInteractiveObject::OnInteraction(APawn* requester, class UPrimitiveComponent* InComp)
{
	int32 Index = 0;
	FTimelineInfo* find = FindTimelineMeshComponent(Cast<UStaticMeshComponent>(InComp), Index);
	if (find)
	{
		if (IsUseTimeline && find->Timeline.IsPlaying())
		{
			return false;
		}

		if (find->CurrentState == EInteractiveObjectState::STATE_OPEN_OR_ON)
		{
			find->CurrentState = EInteractiveObjectState::STATE_CLOSE_OR_OFF;
		}
		else if(find->CurrentState == EInteractiveObjectState::STATE_CLOSE_OR_OFF)
		{
			find->CurrentState = EInteractiveObjectState::STATE_OPEN_OR_ON;
		}
	}

	OnInteractionHappened.ExecuteIfBound();

	if (IsUseTimeline && find)
	{
		NetMulticast_Timeline(Index, find->CurrentState);
	}
	return true;
}
void AInteractiveObject::NetMulticast_Timeline_Implementation(int32 index, EInteractiveObjectState InState)
{
	if (InState == EInteractiveObjectState::STATE_CLOSE_OR_OFF)
	{
		TimelineMeshes[index].Timeline.ReverseFromEnd();
	}
	else if (InState == EInteractiveObjectState::STATE_OPEN_OR_ON)
	{
		switch (TimelineMeshes[index].ControlType)
		{
		case ETimelineControlType::LOCATION_X:
			StartCurveValue =
				TimelineMeshes[index].StaticMeshComponent->GetRelativeLocation().X;
			break;
		case ETimelineControlType::LOCATION_Y:
			StartCurveValue =
				TimelineMeshes[index].StaticMeshComponent->GetRelativeLocation().Y;
			break;
		case ETimelineControlType::LOCATION_Z:
			StartCurveValue =
				TimelineMeshes[index].StaticMeshComponent->GetRelativeLocation().Z;
			break;
		case ETimelineControlType::ROTATION_X:
			StartCurveValue =
				TimelineMeshes[index].StaticMeshComponent->GetRelativeRotation().Roll;
			break;
		case ETimelineControlType::ROTATION_Y:
			StartCurveValue =
				TimelineMeshes[index].StaticMeshComponent->GetRelativeRotation().Yaw;
			break;
		case ETimelineControlType::ROTATION_Z:
			StartCurveValue =
				TimelineMeshes[index].StaticMeshComponent->GetRelativeRotation().Pitch;
			break;
		}
		TimelineMeshes[index].Timeline.PlayFromStart();
	}
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
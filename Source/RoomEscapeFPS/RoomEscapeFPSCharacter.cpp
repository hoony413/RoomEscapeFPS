// Copyright Epic Games, Inc. All Rights Reserved.

#include "RoomEscapeFPSCharacter.h"
#include "RoomEscapeFPSProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "Engine/Classes/Components/SpotLightComponent.h"
#include "TimerManager.h"
#include "Components/SphereComponent.h"
#include "Components/PrimitiveComponent.h"
#include "UI/InteractionPanel.h"
#include "Object/InteractiveObject.h"
#include "Helper/Helper.h"
#include "Managers/UIManager.h"
#include "Net/UnrealNetwork.h"
#include "XRMotionControllerBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// ARoomEscapeFPSCharacter

ARoomEscapeFPSCharacter::ARoomEscapeFPSCharacter()
{
	bReplicates = true;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	// Create a gun mesh component
	Flash = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Flash"));
	Flash->SetOnlyOwnerSee(false);			// otherwise won't be visible in the multiplayer
	Flash->bCastDynamicShadow = false;
	Flash->CastShadow = false;
	Flash->SetupAttachment(RootComponent);

	SpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("FlashSpotLight"));
	SpotLight->SetupAttachment(Flash);
	SpotLight->SetIntensity(100000);

	InteractSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractSphere"));
	InteractSphere->SetupAttachment(FirstPersonCameraComponent);
	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P, FP_Gun, and VR_Gun 
	// are set in the derived blueprint asset named MyCharacter to avoid direct content references in C++.

	// Create VR Controllers.
	R_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("R_MotionController"));
	R_MotionController->MotionSource = FXRMotionControllerBase::RightHandSourceId;
	R_MotionController->SetupAttachment(RootComponent);
	L_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("L_MotionController"));
	L_MotionController->SetupAttachment(RootComponent);
}
void ARoomEscapeFPSCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	Flash->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
	
	SpotLight->AttachToComponent(Flash, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));
	SpotLight->ToggleVisibility();
	
	InteractSphere->SetSphereRadius(350.f);
	
	// Show or hide the two versions of the gun based on whether or not we're using motion controllers.
	if (bUsingMotionControllers)
	{
		Mesh1P->SetHiddenInGame(true, true);
	}
	else
	{
		Mesh1P->SetHiddenInGame(false, true);
	}
}

void ARoomEscapeFPSCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ARoomEscapeFPSCharacter, SphereRadius);
}

//////////////////////////////////////////////////////////////////////////
// Input

void ARoomEscapeFPSCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind Flash event
	PlayerInputComponent->BindAction("Flash", IE_Pressed, this, &ARoomEscapeFPSCharacter::OnFlash);

	// Bind Use event
	PlayerInputComponent->BindAction("Use", IE_Pressed, this, &ARoomEscapeFPSCharacter::OnUse);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &ARoomEscapeFPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARoomEscapeFPSCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ARoomEscapeFPSCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ARoomEscapeFPSCharacter::LookUpAtRate);
}
#if WITH_EDITOR
void ARoomEscapeFPSCharacter::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (PropertyChangedEvent.MemberProperty)
	{
		FName propertyName = PropertyChangedEvent.MemberProperty->GetFName();
		if (propertyName == GET_MEMBER_NAME_CHECKED(ARoomEscapeFPSCharacter, SphereRadius))
		{
			InteractSphere->SetSphereRadius(SphereRadius);
		}
	}
}
#endif
void ARoomEscapeFPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FHitResult result;
	FVector pos = FirstPersonCameraComponent->GetComponentLocation();
	FVector dir = FirstPersonCameraComponent->GetForwardVector();
	FVector end = pos + (dir * ArmRange);

	UWorld* world = GetWorld();
	world->LineTraceSingleByChannel(result, pos, end, ECollisionChannel::ECC_GameTraceChannel2);
	
	bool cachedLooking = IsLooking;
	IsLooking = result.Component.IsValid() && result.Actor.IsValid() &&
		result.Actor.Get()->IsA(InteractableObject) ? true : false;

	if (cachedLooking != IsLooking)
	{
		if (IsLooking)
		{
			cachedInteractObject = result.Actor.Get();
		}
		TurnOnOffWidget(IsLooking);
	}
}
void ARoomEscapeFPSCharacter::OnUse()
{
	ServerOnUse();
}
bool ARoomEscapeFPSCharacter::ServerOnUse_Validate()
{
	return true;
}
void ARoomEscapeFPSCharacter::ServerOnUse_Implementation()
{
	if (HasAuthority())
	{
		// TODO: 범위 안에 있는지 체크
		TSet<AActor*> Actors;
		if (InteractableObject != nullptr)
		{
			InteractSphere->GetOverlappingActors(Actors, InteractableObject);
			for (const auto& elem : Actors)
			{	// 오버랩 액터 추적
				if (cachedInteractObject.IsValid() && cachedInteractObject.Get() == elem)
				{	// 캐시되어 있는(보고 있는) 오브젝트임. Use 처리한다.
					AInteractiveObject* obj = Cast<AInteractiveObject>(cachedInteractObject.Get());
					obj->OnInteraction(obj->GetNextState());
					obj->ToggleState();
					return;
				}
			}
		}
	}
}
void ARoomEscapeFPSCharacter::ChangeInteractText(FName& text)
{
	if (InteractWidget == nullptr)
		return;

	if (InteractWidget)
	{
		InteractWidget->SetText(text);
	}
}
void ARoomEscapeFPSCharacter::TurnOnOffWidget(bool bOnOff)
{
	if (bOnOff == false && InteractWidget == nullptr)
		return;

	if (InteractWidget == nullptr)
	{
		InteractWidget = GetUIMgr()->ShowWidget<UInteractionPanel>();
	}
	if (InteractWidget)
	{
		InteractWidget->SetVisibility(bOnOff ?
			ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	}
}
void ARoomEscapeFPSCharacter::OnFlash()
{
	ServerOnFlash();
}
bool ARoomEscapeFPSCharacter::ServerOnFlash_Validate()
{
	return true;
}
void ARoomEscapeFPSCharacter::ServerOnFlash_Implementation()
{
	if (HasAuthority())
	{
		if (FlashAnimation != nullptr)
		{
			UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
			if (AnimInstance != nullptr)
			{
				if (IsFlash == false)
				{
					AnimInstance->Montage_Play(FlashAnimation, 1.f);
					if (SpotLight)
					{
						GetWorld()->GetTimerManager().ClearTimer(FlashTimer);
						GetWorld()->GetTimerManager().SetTimer(FlashTimer, this,
							&ARoomEscapeFPSCharacter::FlashOnOff, 0.2f, false, 0.2f);
					}
				}
				else
				{
					AnimInstance->Montage_JumpToSection("End");
					GetWorld()->GetTimerManager().ClearTimer(FlashTimer);
					GetWorld()->GetTimerManager().SetTimer(FlashTimer, this,
						&ARoomEscapeFPSCharacter::FlashOnOff, 0.2f, false, 0.2f);
				}
				IsFlash = !IsFlash;
			}
		}
	}
	
}
void ARoomEscapeFPSCharacter::FlashOnOff()
{
	if (SpotLight)
	{
		SpotLight->ToggleVisibility();
	}
}

void ARoomEscapeFPSCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}
void ARoomEscapeFPSCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}
void ARoomEscapeFPSCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}
void ARoomEscapeFPSCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

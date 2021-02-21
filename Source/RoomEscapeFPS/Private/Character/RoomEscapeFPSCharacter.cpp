// Copyright Epic Games, Inc. All Rights Reserved.

#include "Character/RoomEscapeFPSCharacter.h"
//#include "RoomEscapeFPSProjectile.h"
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
#include "Object/GetableObject.h"
#include "Helper/Helper.h"
#include "Managers/UIManager.h"
#include "Gameplay/ProjectileHandler.h"
#include "Object/CharmProjectile.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/RoomEscapeFPSHUD.h"
#include "UI/InventoryPanel.h"
#include "Net/UnrealNetwork.h"


DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// ARoomEscapeFPSCharacter

ARoomEscapeFPSCharacter::ARoomEscapeFPSCharacter()
{
	bReplicates = true;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 100.0f);

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

	CharacterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh"));
	CharacterMesh->SetOwnerNoSee(true);
	CharacterMesh->SetupAttachment(GetCapsuleComponent());
	CharacterMesh->SetRelativeLocation(FVector(0.f, 0.f, -100.f));
	CharacterMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	
	// Create a gun mesh component
	Flash = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Flash"));
	Flash->SetOnlyOwnerSee(false);			// otherwise won't be visible in the multiplayer
	Flash->bCastDynamicShadow = false;
	Flash->CastShadow = false;
	Flash->SetupAttachment(RootComponent);

	SpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("FlashSpotLight"));
	SpotLight->SetupAttachment(Flash);

	InteractSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractSphere"));
	InteractSphere->SetupAttachment(FirstPersonCameraComponent);
	SpotLight->SetIntensity(0.f);
}

void ARoomEscapeFPSCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	if (GetNetMode() == NM_DedicatedServer)
	{
		SphereRadius = ArmRange - 50.f;
		IsFlash = false;
		InteractSphere->SetSphereRadius(SphereRadius);
	}
	
	SpotLight->SetVisibility(false);

	if (IsLocallyControlled())
	{
		Flash->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
	}
	else
	{
		Flash->AttachToComponent(CharacterMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
	}
	SpotLight->AttachToComponent(Flash, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));
}

void ARoomEscapeFPSCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ARoomEscapeFPSCharacter, SphereRadius);
	DOREPLIFETIME(ARoomEscapeFPSCharacter, IsFlash);
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
	
	if (GetNetMode() == NM_Client)
	{
		FHitResult result;
		pos = FirstPersonCameraComponent->GetComponentLocation();
		dir = FirstPersonCameraComponent->GetForwardVector();
		end = pos + (dir * ArmRange);

		UWorld* world = GetWorld();
		world->LineTraceSingleByChannel(result, pos, end, ECollisionChannel::ECC_GameTraceChannel2);

		//bool cachedLooking = IsLooking;
		IsLooking = result.Component.IsValid() && result.Actor.IsValid() &&
			result.Actor.Get()->IsA(InteractableObject) ? true : false;

		//if (cachedLooking != IsLooking)
		//{
			AInteractiveObject* obj = Cast<AInteractiveObject>(result.Actor.Get());
			TurnOnOffWidget(obj, IsLooking);
			
			if (IsLooking)
			{
				AGetableObject* gObj = Cast<AGetableObject>(result.Actor.Get());
				if (gObj && gObj->IsFirstGetNeedsUpdateUI())
				{
					gObj->CaptureCurrentScene();
				}
			}
		//}
	}
}
void ARoomEscapeFPSCharacter::OnUse()
{
	if (!IsLooking)
		return;

	ServerOnUse();
}
bool ARoomEscapeFPSCharacter::ServerOnUse_Validate()
{
	return true;
}
void ARoomEscapeFPSCharacter::ServerOnUse_Implementation()
{
	if (GetNetMode() == NM_DedicatedServer)
	{
		// 서버도 바라보고 있는 물체를 한번 체크한다.
		FHitResult result;
		pos = FirstPersonCameraComponent->GetComponentLocation();
		dir = GetControlRotation().Vector();
		end = pos + (dir * ArmRange);

		GetWorld()->LineTraceSingleByChannel(result, pos, end, ECollisionChannel::ECC_GameTraceChannel3);

		bool bNowLookingActor = 
			result.Component.IsValid() && result.Actor.IsValid() &&
			result.Actor.Get()->IsA(InteractableObject) ? true : false;

		// 범위 안에 있는지 체크
		TSet<AActor*> Actors;
		if (InteractableObject != nullptr && bNowLookingActor)
		{
			InteractSphere->GetOverlappingActors(Actors, InteractableObject);
			AActor* actor = result.Actor.Get();
			for (const auto& elem : Actors)
			{	
				if (elem == actor)
				{
					AInteractiveObject* obj = Cast<AInteractiveObject>(elem);
					obj->OnInteraction(this, result.Component.Get());
					return;
				}
			}
		}
	}
}
void ARoomEscapeFPSCharacter::ChangeInteractText(const FString& str)
{
	if (InteractWidget == nullptr)
		return;

	if (InteractWidget)
	{
		FText txt = FText::FromString(str);
		InteractWidget->SetText(txt);
	}
}
void ARoomEscapeFPSCharacter::TurnOnOffWidget(AInteractiveObject* InObj, bool bOnOff)
{
	if (!IsLocallyControlled() || GetNetMode() != NM_Client)
		return;

	if (InObj == nullptr || bOnOff == false)
	{
		if (InteractWidget != nullptr)
			InteractWidget->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	if (InteractWidget == nullptr)
	{
		InteractWidget = GetUIMgr()->OpenWidget<UInteractionPanel>();
	}
	if (InteractWidget)
	{
		InteractWidget->SetVisibility(bOnOff ?
			ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);

		ChangeInteractText(InObj->GetInformationMessage());
	}
}

void ARoomEscapeFPSCharacter::OnFlash()
{
	if (GetNetMode() == NM_Client)
	{
		ARoomEscapeFPSPlayerState* ps = GetPlayerStateChecked<ARoomEscapeFPSPlayerState>();
		if (ps)
		{
			if (!ps->AmIHaveItem(EItemType::Flash))
				return;
		}

		ServerOnFlash();
	}
}
bool ARoomEscapeFPSCharacter::ServerOnFlash_Validate()
{
	return true;
}
void ARoomEscapeFPSCharacter::ServerOnFlash_Implementation()
{
	if (GetNetMode() == NM_DedicatedServer)
	{
		// TODO: 후레쉬를 가지고 있지 않은 경우 리턴.
		ARoomEscapeFPSPlayerState* ps = GetPlayerStateChecked<ARoomEscapeFPSPlayerState>();
		if (ps)
		{
			if (ps->AmIHaveItem(EItemType::Flash))
			{
				IsFlash = !IsFlash;
				ps->ToggleBatteryReduceState(IsFlash);
			}
		}
	}
}
void ARoomEscapeFPSCharacter::OnRep_IsFlash()
{
	FlashToggleAnimation();
}
void ARoomEscapeFPSCharacter::ToggleFlash()
{
	if (SpotLight)
	{
		SpotLight->ToggleVisibility();
	}
}

void ARoomEscapeFPSCharacter::UpdateFlashIntensity(float InIntensity)
{
	if (SpotLight)
	{
		SpotLight->SetIntensity(InIntensity);
	}
}

void ARoomEscapeFPSCharacter::OnFire()
{
	if (GetNetMode() == NM_Client)
	{
		ARoomEscapeFPSPlayerState* ps = GetPlayerStateChecked<ARoomEscapeFPSPlayerState>();
		if (!ps->AmIHaveItem(EItemType::Charm))
			return;

		ARoomEscapeFPSHUD* hud = Cast<ARoomEscapeFPSHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
		if (hud)
		{	// 부적 카운트 업데이트(UI)
			hud->GetInventoryPanel()->UpdateCharmCount(ps->GetItemCount(EItemType::Charm) - 1);
		}

		ServerOnFire();
	}
}
bool ARoomEscapeFPSCharacter::ServerOnFire_Validate()
{
	return true;
}
void ARoomEscapeFPSCharacter::ServerOnFire_Implementation()
{
	if (GetNetMode() == NM_DedicatedServer)
	{
		// TODO: 부적을 가지고 있지 않은 경우 리턴.
		ARoomEscapeFPSPlayerState* ps = GetPlayerStateChecked<ARoomEscapeFPSPlayerState>();
		if (ps)
		{
			if (!ps->AmIHaveItem(EItemType::Charm))
				return;

			ps->AddItemToInventory(EItemType::Charm, -1);
		}
		// TODO: 부적 탄체 생성, 플레이어로부터 발사 처리.
		if (!cachedProjectileHandlerPtr.IsValid())
		{
			cachedProjectileHandlerPtr = Helper::GetProjectileHandler(GetWorld());
		}
		
		check(cachedProjectileHandlerPtr.IsValid());

		ACharmProjectile* proj = cachedProjectileHandlerPtr.Get()->GetCharm(this);
		if (proj)
		{
			const FRotator SpawnRotation = GetControlRotation();
			////Set Spawn Collision Handling Override
			//FActorSpawnParameters ActorSpawnParams;
			//ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			// 탄체 시작위치: 캐릭터 발 위치 + 캐릭터 방향벡터 * 스칼라 값(50) 
			FVector FirePosition = 
				GetCharacterMovement()->GetActorFeetLocation() + 
				(GetActorForwardVector() * 50);
			FirePosition.Z += 120.f;

			// 격발은 Netmulticast로.
			proj->Fire(FirePosition, SpawnRotation.Vector());
		}
	}
}
void ARoomEscapeFPSCharacter::FlashToggleAnimation()
{
	if (FlashAnimation != nullptr)
	{
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			if (IsFlash == false)
			{
				AnimInstance->Montage_JumpToSection("End");
				GetWorld()->GetTimerManager().ClearTimer(FlashTimer);
				GetWorld()->GetTimerManager().SetTimer(FlashTimer, this,
					&ARoomEscapeFPSCharacter::ToggleFlash, 0.2f, false, 0.2f);
			}
			else
			{
				AnimInstance->Montage_Play(FlashAnimation, 1.f);
				GetWorld()->GetTimerManager().ClearTimer(FlashTimer);
				GetWorld()->GetTimerManager().SetTimer(FlashTimer, this,
					&ARoomEscapeFPSCharacter::ToggleFlash, 0.2f, false, 0.2f);
			}
		}
	}
}

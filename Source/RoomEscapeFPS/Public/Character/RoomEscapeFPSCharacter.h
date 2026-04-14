// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RoomEscapeFPSCharacter.generated.h"

class ACharmProjectile;
class USpotLightComponent;
class USphereComponent;
class UInputComponent;
class USceneComponent;
class UCameraComponent;
class UMotionControllerComponent;
class USoundBase;

UCLASS(config=Game)
class ARoomEscapeFPSCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	TObjectPtr<USkeletalMeshComponent> Mesh1P;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	TObjectPtr<USkeletalMeshComponent> CharacterMesh;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	TObjectPtr<UStaticMeshComponent> Flash;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	TObjectPtr<USpotLightComponent> SpotLight;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FirstPersonCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InteractSpehere", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> InteractSphere;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSoftClassPtr<ACharmProjectile> CharmClass;
	
public:
	ARoomEscapeFPSCharacter();

	void Tick(float DeltaTime) override;

#if WITH_EDITOR
	void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:
	void BeginPlay() override;

	UFUNCTION()
	void TurnOnOffWidget(class AInteractiveObject* InObj, bool bOnOff);

public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	TObjectPtr<UAnimMontage> FlashAnimation;

	/** Whether to use motion controller location for aiming. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	uint8 bUsingMotionControllers : 1;

	UPROPERTY(EditAnywhere, Category = "Trace Scalar")
	float ArmRange;

	UFUNCTION()
	void OnUse();

	UFUNCTION()
	void OnFlash();

	UFUNCTION()
	void OnFire();

	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	FORCEINLINE USphereComponent* GetInteractSphere() const { return InteractSphere; }

	void UpdateFlashIntensity(float InIntensity);

protected:
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerOnUse();

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerOnFlash();

	UFUNCTION(Reliable, NetMulticast)
	void MulticastOnFlashToggled(bool bIsFlash);

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerOnFire();

	void ToggleFlash();

	void FlashToggleAnimation(bool bIsFlashOn);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InteractSpehere", Meta = (AllowPrivateAccess = "true"))
	float SphereRadius;
	
	bool IsFlash = false;
	bool IsLooking = false;
};


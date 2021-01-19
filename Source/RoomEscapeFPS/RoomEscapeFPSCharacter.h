// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RoomEscapeFPSCharacter.generated.h"

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
		class USkeletalMeshComponent* Mesh1P;
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USkeletalMeshComponent* CharacterMesh;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	UStaticMeshComponent* Flash;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USpotLightComponent* SpotLight;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InteractSpehere", meta = (AllowPrivateAccess = "true"))
		class USphereComponent* InteractSphere;
	
	/** Motion controller (right hand) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UMotionControllerComponent* R_MotionController;

	/** Motion controller (left hand) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UMotionControllerComponent* L_MotionController;
	
public:
	ARoomEscapeFPSCharacter();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Tick(float DeltaTime) override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:
	virtual void BeginPlay();

	void ChangeInteractText(FName& text);
	void TurnOnOffWidget(bool bOnOff);

public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* FlashAnimation;

	/** Whether to use motion controller location for aiming. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	uint8 bUsingMotionControllers : 1;

	UPROPERTY(EditAnywhere, Category = "Trace Scalar")
		float ArmRange;

	UPROPERTY()
		TSoftObjectPtr<class UInteractionPanel> InteractWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact C++ class")
		TSubclassOf<class AInteractiveObject> InteractableObject;

	UPROPERTY()
		TWeakObjectPtr<class AActor> cachedInteractObject;

	FTimerHandle FlashTimer;

protected:
	UFUNCTION()
	void OnUse();
	UFUNCTION(Reliable, Server, WithValidation)
		void ServerOnUse();

	UFUNCTION()
	void OnFlash();
	UFUNCTION(Reliable, Server, WithValidation)
		void ServerOnFlash();
	UFUNCTION(NetMulticast, Unreliable)
		void NetMulticast_ToggleFlash();

	UFUNCTION()
	void FlashToggleAnimation();

	//UFUNCTION()
	//void FlashOnOff();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

private:
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "InteractSpehere", Meta = (AllowPrivateAccess = "true"))
		float SphereRadius;
	
	UPROPERTY(Replicated)
	bool IsFlash = false;

	bool IsLooking = false;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	/* 
	 * Configures input for touchscreen devices if there is a valid touch interface for doing so 
	 *
	 * @param	InputComponent	The input component pointer to bind controls to
	 * @returns true if touch controls were enabled.
	 */

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	FORCEINLINE class USphereComponent* GetInteractSphere() const { return InteractSphere; }
};


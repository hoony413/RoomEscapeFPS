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

USTRUCT()
struct ROOMESCAPEFPS_API FLookingObjectInfo
{
	GENERATED_BODY()

public:
	FLookingObjectInfo() {}

	TWeakObjectPtr<class AActor> cachedInteractObject;
};

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
	
public:
	ARoomEscapeFPSCharacter();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Tick(float DeltaTime) override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:
	virtual void BeginPlay();

	void ChangeInteractText(const FString& str);

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

	FTimerHandle FlashTimer;

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

	FORCEINLINE class USphereComponent* GetInteractSphere() const { return InteractSphere; }

	void UpdateFlashIntensity(float InIntensity);

protected:
	UFUNCTION(Reliable, Server, WithValidation)
		void ServerOnUse();
	UFUNCTION(Reliable, Server, WithValidation)
		void ServerOnFlash();
	UFUNCTION()
		void OnRep_IsFlash();

	UFUNCTION(Reliable, Server, WithValidation)
		void ServerOnFire();

	void ToggleFlash();
	UFUNCTION()
		void FlashToggleAnimation();

private:
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "InteractSpehere", Meta = (AllowPrivateAccess = "true"))
		float SphereRadius;
	
	UPROPERTY(ReplicatedUsing = OnRep_IsFlash)
	bool IsFlash = false;
	bool IsLooking = false;

	TWeakObjectPtr<class AProjectileHandler> cachedProjectileHandlerPtr;

	FVector pos;
	FVector dir;
	FVector end;
};


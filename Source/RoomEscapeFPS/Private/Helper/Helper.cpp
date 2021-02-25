// Fill out your copyright notice in the Description page of Project Settings.


#include "Helper/Helper.h"
#include "Runtime/Engine/Classes/Engine/AssetManager.h"
//#include "Paper2D/Classes/PaperSprite.h"

namespace Helper
{
	ROOMESCAPEFPS_API TSharedPtr<FStreamableHandle> AsyncLoadResource(const FSoftObjectPath& assetRef, TFunction<void()>&& lambda)
	{
		FStreamableManager& assetLoader = UAssetManager::GetStreamableManager();
		return assetLoader.RequestAsyncLoad(TArray<FSoftObjectPath>{ assetRef }, FStreamableDelegate::CreateLambda(MoveTemp(lambda)));
	}

	ROOMESCAPEFPS_API AProjectileHandler* GetProjectileHandler(UWorld* world)
	{	// TODO: 맵에 하나만 존재하므로 일단 이렇게 처리. 추후 예외처리는 고민.
		for (TActorIterator<AProjectileHandler> it(world); it; ++it)
		{
			return *it;
		}
		return nullptr;
	}

	ROOMESCAPEFPS_API void SetActorActive(class AActor* InActor, bool bActive)
	{
		ensure(InActor);

		// 액터를 Hidden 처리
		InActor->SetActorHiddenInGame(!bActive);
		
		// 액터의 충돌 검출 끄기
		InActor->SetActorEnableCollision(bActive);
		
		// 액터 틱 끄기
		InActor->SetActorTickEnabled(bActive);
		TArray<UActorComponent*> components;
		InActor->GetComponents(components);
		for (UActorComponent* c : components)
		{
			c->SetActive(bActive);
		}
	}

	ROOMESCAPEFPS_API EServerSolutionType GetSolutionType(EServerSolutionResultType InType)
	{
		switch (InType)
		{
		case EServerSolutionResultType::ESolutionResult_1:
			return EServerSolutionType::ESolution_1;
		case EServerSolutionResultType::ESolutionResult_2:
			return EServerSolutionType::ESolution_2;
		}
		return EServerSolutionType::ENONE;
	}
	ROOMESCAPEFPS_API EServerSolutionResultType GetSolutionResultType(EServerSolutionType InType)
	{
		switch (InType)
		{
		case EServerSolutionType::ESolution_1:
			return EServerSolutionResultType::ESolutionResult_1;
		case EServerSolutionType::ESolution_2:
			return EServerSolutionResultType::ESolutionResult_2;
		}
		return EServerSolutionResultType::ENONE;
	}

	// Formula to convert a Euler angle in degrees to a quaternion rotation
	ROOMESCAPEFPS_API FQuat Euler_To_Quaternion(FRotator& Current_Rotation)
	{
		FQuat q;                                            // Declare output quaternion
		float yaw = Current_Rotation.Yaw * PI / 180;        // Convert degrees to radians 
		float roll = Current_Rotation.Roll * PI / 180;
		float pitch = Current_Rotation.Pitch * PI / 180;

		double cy = cos(yaw * 0.5);
		double sy = sin(yaw * 0.5);
		double cr = cos(roll * 0.5);
		double sr = sin(roll * 0.5);
		double cp = cos(pitch * 0.5);
		double sp = sin(pitch * 0.5);

		q.W = cy * cr * cp + sy * sr * sp;
		q.X = cy * sr * cp - sy * cr * sp;
		q.Y = cy * cr * sp + sy * sr * cp;
		q.Z = sy * cr * cp - cy * sr * sp;

		return q;                                           // Return the quaternion of the input Euler rotation
	}

	// Set the scene component's world rotation to the input quaternion
	ROOMESCAPEFPS_API void SetWorldRotationQuat(USceneComponent* SceneComponent, const FQuat& Desired_Rotation)
	{
		if (SceneComponent)
		{
			SceneComponent->SetWorldRotation(Desired_Rotation);
		}
	}

	// Set the scene component's relative rotation to the input quaternion
	ROOMESCAPEFPS_API void SetRelativeRotationQuat(USceneComponent* SceneComponent, const FQuat& Desired_Rotation)
	{
		if (SceneComponent)
		{
			SceneComponent->SetRelativeRotation(Desired_Rotation);
		}
	}

	// Add the input delta rotation to the scene component's current local rotation
	ROOMESCAPEFPS_API void AddLocalRotationQuat(USceneComponent* SceneComponent, const FQuat& Delta_Rotation)
	{
		if (SceneComponent)
		{
			SceneComponent->AddLocalRotation(Delta_Rotation);
		}
	}

	// Set the Actor's world rotation to the input quaternion
	ROOMESCAPEFPS_API void SetActorWorldRotationQuat(AActor* Actor, const FQuat& Desired_Rotation)
	{
		if (Actor)
		{
			Actor->SetActorRotation(Desired_Rotation);
		}
	}

	// Set the Actor's relative rotation to the input quaternion
	ROOMESCAPEFPS_API void SetActorRelativeRotationQuat(AActor* Actor, const FQuat& Desired_Rotation)
	{
		if (Actor)
		{
			Actor->SetActorRelativeRotation(Desired_Rotation);
		}
	}

	// Add the input delta rotation to the Actor's current local rotation
	ROOMESCAPEFPS_API void AddActorLocalRotationQuat(AActor* Actor, const FQuat& Delta_Rotation)
	{
		if (Actor)
		{
			Actor->AddActorLocalRotation(Delta_Rotation);
		}
	}
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/GetableObject.h"
#include "GameFramework/RoomEscapeFPSPlayerState.h"
#include "GameFramework/RoomEscapeFPSPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Gameplay/TypeInfoHeader.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Components/PointLightComponent.h"
#include "Helper/Helper.h"

AGetableObject::AGetableObject()
{
	if (not IsRunningDedicatedServer())
	{
		SceneCapturer = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCapture2D"));
		SceneCapturer->ProjectionType = ECameraProjectionMode::Perspective;
		SceneCapturer->FOVAngle = 90.f;
		static ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> RTAsset(TEXT("TextureRenderTarget2D'/Game/Blueprints/Gameplay/InventoryItemRenderTarget.InventoryItemRenderTarget'"));
		if (RTAsset.Succeeded())
		{
			SceneCapturer->TextureTarget = RTAsset.Object;
		}

		SceneCapturer->CompositeMode = SCCM_Overwrite;
		SceneCapturer->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;
		SceneCapturer->bCaptureEveryFrame = false;
		SceneCapturer->bCaptureOnMovement = false;
		SceneCapturer->MaxViewDistanceOverride = -1.f;
		SceneCapturer->SetupAttachment(DefaultMesh);

		SceneCapturer->ShowFlags.SetFog(false);
		SceneCapturer->ShowFlags.SetVolumetricFog(false);
		SceneCapturer->ShowFlags.SetDynamicShadows(false);
		SceneCapturer->ShowFlags.SetAtmosphere(false);
		SceneCapturer->ShowFlags.SetBloom(false);
		SceneCapturer->ShowFlags.SetMotionBlur(false);
		SceneCapturer->ShowFlags.SetEyeAdaptation(false);

		SceneCapturer->PostProcessBlendWeight = 1.0f;
		SceneCapturer->PostProcessSettings.bOverride_AutoExposureMethod = true;
		SceneCapturer->PostProcessSettings.AutoExposureMethod = EAutoExposureMethod::AEM_Manual;
		SceneCapturer->PostProcessSettings.bOverride_AutoExposureBias = true;
		SceneCapturer->PostProcessSettings.AutoExposureBias = 10.0f;

		CaptureLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("CaptureLight"));
		CaptureLight->SetupAttachment(SceneCapturer);
		CaptureLight->Intensity = 0.f;
		CaptureLight->AttenuationRadius = 300.f;
		CaptureLight->CastShadows = false;
	}

	bNeedsUINotify = true;
}

void AGetableObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AGetableObject::BeginPlay()
{
	Super::BeginPlay();

	// 블루프린트에서 추가한 컴포넌트는 생성자에서 검색되지 않는다. BeginPlay에서 설정하면 됨.
	TArray<UStaticMeshComponent*> actors;
	GetComponents<UStaticMeshComponent>(actors);
	for (auto const& elem : actors)
	{
		if (elem->IsA<UStaticMeshComponent>())
		{
			UStaticMeshComponent* mesh = Cast<UStaticMeshComponent>(elem);
			if (SceneCapturer)
			{
				SceneCapturer->ShowOnlyComponent(mesh);
			}
			mesh->SetCollisionProfileName(FName(TEXT("ServerInteraction")));
		}
	}


	//InformationStr = TEXT("Press 'E' key to get");
}

bool AGetableObject::OnInteraction(APawn* requester, class UPrimitiveComponent* InComp)
{
	if (IsNetMode(NM_DedicatedServer))
	{
		check(requester);
		ARoomEscapeFPSPlayerState* ps = requester->GetPlayerStateChecked<ARoomEscapeFPSPlayerState>();
		if (ItemType == EItemType::FLASH && not ps->IsFirstGet(ItemType))
		{
			return false;
		}

		Helper::SetActorActive(this, false);
		int32 id = ps->GetPlayerId();
		auto const AddItemToPlayerInventory = [this, &ps](ARoomEscapeFPSPlayerController* pc)
		{
			// 튜토리얼 UI를 켜줘야 하는 특수 아이템타입에 대한 처리.
			if (ps->IsFirstGet(ItemType) && bNeedsUINotify)
			{
				pc->ClientProcessHUDOnFirstItemGet(this);
			}
			if (AdditionalItemType != EItemType::NONE)
			{	// 후레쉬 획득의 경우, 후레쉬 획득과 함께 배터리도 일부 충전해줘야 한다.
				ps->AddItemToInventory(AdditionalItemType, AdditionalGetCount);
			}
			ps->AddItemToInventory(ItemType, DefaultGetCount);
		};
		Helper::ServerImplementToClient(GetWorld(), id, AddItemToPlayerInventory);

		if (ItemType == EItemType::FLASH)
		{
			Helper::UpdateNextUIInfo(GetWorld(), ENextInformationType::FIND_LANTERN, ENextInformationType::SOLVE_CLUE_1, 1);
		}
		Destroy();
	}

	return true;
}

void AGetableObject::CaptureCurrentScene()
{
	if (not SceneCapturer)
	{
		return;
	}

	if (CaptureLight)
	{
		CaptureLight->SetIntensity(5000.f);
	}

	SceneCapturer->CaptureScene();

	if (CaptureLight)
	{
		CaptureLight->SetIntensity(0.f);
	}
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/GetableObject.h"
#include "GameFramework/RoomEscapeFPSPlayerState.h"
#include "GameFramework/Pawn.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Gameplay/TypeInfoHeader.h"
#include "Engine/Classes/Engine/TextureRenderTarget2D.h"
#include "Engine/Classes/Engine/CanvasRenderTarget2D.h"
#include "Helper/Helper.h"

AGetableObject::AGetableObject()
{
	SceneCapturer = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCapture2D"));
	SceneCapturer->ProjectionType = ECameraProjectionMode::Perspective;
	SceneCapturer->FOVAngle = 90.f;
	static ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> RTAsset(TEXT("TextureRenderTarget2D'/Game/Resources/Blueprints/Gameplay/InventoryItemRenderTarget.InventoryItemRenderTarget'"));
	if (RTAsset.Succeeded())
	{
		SceneCapturer->TextureTarget = RTAsset.Object;
	}

	SceneCapturer->CompositeMode = ESceneCaptureCompositeMode::SCCM_Overwrite;
	SceneCapturer->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;
	SceneCapturer->bCaptureEveryFrame = false;
	SceneCapturer->bCaptureOnMovement = false;
	SceneCapturer->MaxViewDistanceOverride = -1.f;
	SceneCapturer->SetupAttachment(DefaultMesh);
}

void AGetableObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AGetableObject::BeginPlay()
{
	Super::BeginPlay();
	if (GetNetMode() == NM_DedicatedServer)
	{
		IsUseTimeline = false;
	}

	// �������Ʈ���� �߰��� ������Ʈ�� �����ڿ��� �˻����� �ʴ´�. BeginPlay���� �����ϸ� ��.
	TArray<UStaticMeshComponent*> actors;
	GetComponents<UStaticMeshComponent>(actors);
	for (const auto& elem : actors)
	{
		if (elem->IsA<UStaticMeshComponent>())
		{
			UStaticMeshComponent* mesh = Cast<UStaticMeshComponent>(elem);
			SceneCapturer->ShowOnlyComponent(mesh);
			mesh->SetCollisionProfileName(FName(TEXT("ServerInteraction")));
		}
	}

	InformationStr = TEXT("Press 'E' key to get");
}

void AGetableObject::OnInteraction(APawn* requester, class UPrimitiveComponent* InComp)
{
	if (GetNetMode() == NM_DedicatedServer)
	{
		check(requester);
		Helper::SetActorActive(this, false);
		
		ARoomEscapeFPSPlayerState* ps = requester->GetPlayerStateChecked<ARoomEscapeFPSPlayerState>();
		int32 id = ps->GetPlayerId();
		auto AddItemToPlayerInventory = [this, &ps]()
		{
			// UI�� ����� �ϴ� Ư�� ������Ÿ�Կ� ���� ó��.
			if (ps->IsFirstGet(ItemType) && bFirstGetNeedsUpdateUI)
			{
				ps->ClientProcessHUDOnFirstItemGet(this);
			}
			if (AdditionalItemType != EItemType::NONE)
			{	// �ķ��� ȹ���� ���, �ķ��� ȹ��� �Բ� ���͸��� �Ϻ� ��������� �Ѵ�.
				ps->AddItemToInventory(AdditionalItemType, AdditionalGetCount);
			}
			ps->AddItemToInventory(ItemType, DefaultGetCount);
		};
		Helper::ServerImplementToClient(GetWorld(), id, AddItemToPlayerInventory);
		Destroy();
	}
}

void AGetableObject::CaptureCurrentScene()
{
	if (SceneCapturer)
	{
		SceneCapturer->CaptureScene();
	}
}
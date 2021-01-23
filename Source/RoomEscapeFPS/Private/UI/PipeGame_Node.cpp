// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PipeGame_Node.h"
#include "GameFramework/RoomEscapeFPSPlayerState.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Helper/Helper.h"

void UPipeGame_Node::InitializePipeNode(FPipeNode& InNode)
{
	PipeNodeRef = InNode;
	SetWidgetAnimation();
	// 회전 버튼 바인딩
	if (PipeButton->OnClicked.IsBound() == false)
	{
		PipeButton->OnClicked.AddDynamic(this, &UPipeGame_Node::OnClickedPipeButton);
	}

	// 전송된 노드 데이터 기준으로 이미지 생성 및 회전
	EPipeType type = PipeNodeRef.GetPipeType();
	FString str;
	if (type == EPipeType::EStraight_Two)
	{
		UTexture2D* tex = Helper::SyncLoadResource<UTexture2D>(TEXT("Texture2D'/Game/Resources/Widgets/Images/two_straight_t.two_straight_t'"));//Straight_Two.LoadSynchronous();
		PipeImage->SetBrushFromTexture(tex);
		
		// 상-하 이거나 좌-우인지만 확인하면 됨.
		if (PipeNodeRef.IsContainDirection(EPipeDirection::ELeft))
		{
			RotationInfo = 1u;
			PipeButton->SetRenderTransformAngle(90.f);
		}
	}
	else if (type == EPipeType::ETwo)
	{
		UTexture2D* tex = Helper::SyncLoadResource<UTexture2D>(TEXT("Texture2D'/Game/Resources/Widgets/Images/two_t.two_t'"));
		//UTexture2D* tex = Two.LoadSynchronous();
		PipeImage->SetBrushFromTexture(tex);

		uint8 i = 0u;
		while (true)
		{
			uint8 a = 1 << i;
			uint8 b = 1 << (i + 1);
			if (PipeNodeRef.IsCorrectDirection(a + b))
			{
				RotationInfo = i;
				PipeButton->SetRenderTransformAngle(90u * i);
				break;
			}
			else
			{
				i++;
			}
		}
	}
	else if (type == EPipeType::EThree)
	{
		UTexture2D* tex = Helper::SyncLoadResource<UTexture2D>(TEXT("Texture2D'/Game/Resources/Widgets/Images/three_t.three_t'"));
		//UTexture2D* tex = Three.LoadSynchronous();
		PipeImage->SetBrushFromTexture(tex);

		uint8 i = 0u;
		while (true)
		{
			if (!PipeNodeRef.IsContainDirection((EPipeDirection)(1 << i)))
			{
				RotationInfo = i;
				PipeButton->SetRenderTransformAngle(90u * i);
				break;
			}
			else
			{
				i++;
			}
		}
	}
	else if (type == EPipeType::EFour)
	{
		// 회전 필요 없음.
		UTexture2D* tex = Helper::SyncLoadResource<UTexture2D>(TEXT("Texture2D'/Game/Resources/Widgets/Images/four_t.four_t'"));
		//UTexture2D* tex = Four.LoadSynchronous();
		PipeImage->SetBrushFromTexture(tex);
		RotationInfo = 0u;
	}

	PipeImage->SetBrushFromTexture(Two.Get());
}
void UPipeGame_Node::OnClickedPipeButton()
{
	// 현재 회전값을 기준으로 회전 애니메이션 출력
	PlayAnimation(AnimArray[RotationInfo]);
	// 원본 데이터 회전.
	PipeNodeRef.RotatePipe();
	// 회전값 증가
	RotationInfo += RotationInfo == 3 ? -3 : 1;
}
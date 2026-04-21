// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PipeGame_Node.h"
#include "GameFramework/RoomEscapeFPSPlayerController.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Paper2D/Classes/PaperSprite.h"
#include "UI/PipeGamePanel.h"
#include "Helper/Helper.h"

void UPipeGame_Node::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	Super::OnAnimationFinished_Implementation(Animation);
}

void UPipeGame_Node::InitializePipeNode(FPipeNode const& InNode, uint8 InGridSize)
{
	PipeNodeRef = InNode;
	GridSize = InGridSize;

	SetWidgetAnimation();
	
	// 회전 버튼 바인딩
	PipeButton->OnClicked.AddDynamic(this, &UPipeGame_Node::OnClickedPipeButton);

	// 전송된 노드 데이터 기준으로 이미지 생성 및 회전
	EPipeType type = PipeNodeRef.GetPipeType();
	UPaperSprite* spr = nullptr;
	if (type == EPipeType::STRAIGHT_TWO)
	{
		spr = Straight_Two.LoadSynchronous();

		// 상-하 이거나 좌-우인지만 확인하면 됨.
		if (PipeNodeRef.IsContainDirection(EPipeDirection::LEFT))
		{
			RotationInfo = 1u;
			PipeButton->SetRenderTransformAngle(90.f);
		}
	}
	else if (type == EPipeType::TWO)
	{
		spr = Two.LoadSynchronous();
		uint8 i = 0u;

		while (true)
		{
			uint8 a = 1 << i;
			uint8 b = 1 << (i + 1);
			if (i + 1 >= static_cast<uint8>(EPipeType::MAX))
			{
				b = 1 << 0;
			}
			if (PipeNodeRef.IsContainDirection(static_cast<EPipeDirection>(a)) &&
				PipeNodeRef.IsContainDirection(static_cast<EPipeDirection>(b)))
			{
				RotationInfo = i % static_cast<uint8>(EPipeType::MAX);
				PipeButton->SetRenderTransformAngle(90u * i);
				break;
			}

			i++;
		}
	}
	else if (type == EPipeType::THREE)
	{
		spr = Three.LoadSynchronous();
		uint8 i = 0u;

		while (true)
		{
			if (!PipeNodeRef.IsContainDirection(static_cast<EPipeDirection>(1 << i)))
			{
				RotationInfo = i % static_cast<uint8>(EPipeType::MAX);
				PipeButton->SetRenderTransformAngle(90u * i);
				break;
			}
			i++;
		}
	}
	else if (type == EPipeType::FOUR)
	{
		spr = Four.LoadSynchronous();
		// 회전 필요 없음.
		RotationInfo = 0u;
	}

	if (spr)
	{
		FSlateBrush NewBrush = PipeImage->GetBrush();
		NewBrush.SetResourceObject(spr);
		PipeImage->SetBrush(NewBrush);
	}
}
void UPipeGame_Node::OnClickedPipeButton()
{
	// 현재 회전값을 기준으로 회전 애니메이션 출력
	if (IsPlayingAnimation())
		return;

	ARoomEscapeFPSPlayerController* pc = GetOwningPlayer<ARoomEscapeFPSPlayerController>();
	if (pc)
	{	// 회전(서버)
		int32 index = (PipeNodeRef.GetPipeLocation().Y * GridSize) + PipeNodeRef.GetPipeLocation().X;
		pc->ServerRotatePipe(index);
	}

	PlayAnimation(AnimArray[RotationInfo]);

	// 회전(클라)
	PipeNodeRef.RotatePipe();
	RotationInfo++;
	RotationInfo %= static_cast<uint8>(EPipeType::MAX);
}
void UPipeGame_Node::PlayResultAnimation()
{
	int32 const index = static_cast<int32>(EAnimationIndex::RESULT);
	if (AnimArray.IsValidIndex(index))
	{
		PlayAnimation(AnimArray[index]);
	}
}
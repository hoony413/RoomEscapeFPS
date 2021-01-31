// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PipeGame_Node.h"
#include "GameFramework/RoomEscapeFPSPlayerState.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Paper2D/Classes/PaperSprite.h"
#include "Helper/Helper.h"

void UPipeGame_Node::InitializePipeNode(FPipeNode& InNode)
{
	PipeNodeRef = InNode;
	SetWidgetAnimation();
	// 회전 버튼 바인딩
	PipeButton->OnClicked.AddDynamic(this, &UPipeGame_Node::OnClickedPipeButton);

	// 전송된 노드 데이터 기준으로 이미지 생성 및 회전
	EPipeType type = PipeNodeRef.GetPipeType();
	UPaperSprite* spr = nullptr;
	if (type == EPipeType::EStraight_Two)
	{
		spr = Straight_Two.LoadSynchronous();
		
		// 상-하 이거나 좌-우인지만 확인하면 됨.
		if (PipeNodeRef.IsContainDirection(EPipeDirection::ELeft))
		{
			RotationInfo = 1u;
			PipeButton->SetRenderTransformAngle(90.f);
		}
	}
	else if (type == EPipeType::ETwo)
	{
		spr = Two.LoadSynchronous();
		uint8 i = 0u;
		
		while (true)
		{
			uint8 a = 1 << i;
			uint8 b = 1 << (i + 1);
			if (PipeNodeRef.IsCorrectDirection(a + b))
			{
				RotationInfo = i % (uint8)EPipeType::EMAX;
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
		spr = Three.LoadSynchronous();
		uint8 i = 0u;
		
		while (true)
		{
			if (!PipeNodeRef.IsContainDirection((EPipeDirection)(1 << i)))
			{
				RotationInfo = i % (uint8)EPipeType::EMAX;
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
		spr = Four.LoadSynchronous();
		RotationInfo = 0u;
	}

	PipeImage->Brush.SetResourceObject(spr);
}
void UPipeGame_Node::OnClickedPipeButton()
{
	// 현재 회전값을 기준으로 회전 애니메이션 출력
	if (IsPlayingAnimation())
		return;

	PlayAnimation(AnimArray[RotationInfo]);
	// 원본 데이터 회전.
	PipeNodeRef.RotatePipe();
	// 회전값 증가
	RotationInfo++;
	RotationInfo %= (uint8)EPipeType::EMAX;
}
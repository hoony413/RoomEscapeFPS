// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PipeGame_Node.h"
#include "GameFramework/RoomEscapeFPSPlayerState.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Paper2D/Classes/PaperSprite.h"
#include "UI/PipeGameUI.h"
#include "Helper/Helper.h"

void UPipeGame_Node::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	Super::OnAnimationFinished_Implementation(Animation);
}

void UPipeGame_Node::InitializePipeNode(FPipeNode& InNode, uint8 InGridSize)
{
	PipeNodeRef = InNode;
	GridSize = InGridSize;

	SetWidgetAnimation();
	
	// ȸ�� ��ư ���ε�
	PipeButton->OnClicked.AddDynamic(this, &UPipeGame_Node::OnClickedPipeButton);

	// ���۵� ��� ������ �������� �̹��� ���� �� ȸ��
	EPipeType type = PipeNodeRef.GetPipeType();
	UPaperSprite* spr = nullptr;
	if (type == EPipeType::EStraight_Two)
	{
		spr = Straight_Two.LoadSynchronous();
		
		// ��-�� �̰ų� ��-�������� Ȯ���ϸ� ��.
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
			if (i + 1 >= (uint8)EPipeType::EMAX)
			{
				b = 1 << 0;
			}
			if (PipeNodeRef.IsContainDirection((EPipeDirection)a) &&
				PipeNodeRef.IsContainDirection((EPipeDirection)b))
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
		// ȸ�� �ʿ� ����.
		spr = Four.LoadSynchronous();
		RotationInfo = 0u;
	}

	PipeImage->Brush.SetResourceObject(spr);
}
void UPipeGame_Node::OnClickedPipeButton()
{
	// ���� ȸ������ �������� ȸ�� �ִϸ��̼� ���
	if (IsPlayingAnimation())
		return;

	ARoomEscapeFPSPlayerState* ps = GetOwningPlayerState<ARoomEscapeFPSPlayerState>(true);
	if (ps)
	{	// ȸ��(����)
		int32 index = (PipeNodeRef.GetPipeLocation().Y * GridSize) + PipeNodeRef.GetPipeLocation().X;
		ps->ServerRotatePipe(index);
	}

	PlayAnimation(AnimArray[RotationInfo]);

	// ȸ��(Ŭ��)
	PipeNodeRef.RotatePipe();
	RotationInfo++;
	RotationInfo %= (uint8)EPipeType::EMAX;
}
void UPipeGame_Node::PlayResultAnimation()
{
	PlayAnimation(AnimArray[(int32)EAnimationIndex::EResult]);
}
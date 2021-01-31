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

	PlayAnimation(AnimArray[RotationInfo]);
	// ���� ������ ȸ��.
	PipeNodeRef.RotatePipe();
	// ȸ���� ����
	RotationInfo++;
	RotationInfo %= (uint8)EPipeType::EMAX;
}
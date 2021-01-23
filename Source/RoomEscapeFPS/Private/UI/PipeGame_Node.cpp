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
	// ȸ�� ��ư ���ε�
	if (PipeButton->OnClicked.IsBound() == false)
	{
		PipeButton->OnClicked.AddDynamic(this, &UPipeGame_Node::OnClickedPipeButton);
	}

	// ���۵� ��� ������ �������� �̹��� ���� �� ȸ��
	EPipeType type = PipeNodeRef.GetPipeType();
	FString str;
	if (type == EPipeType::EStraight_Two)
	{
		UTexture2D* tex = Helper::SyncLoadResource<UTexture2D>(TEXT("Texture2D'/Game/Resources/Widgets/Images/two_straight_t.two_straight_t'"));//Straight_Two.LoadSynchronous();
		PipeImage->SetBrushFromTexture(tex);
		
		// ��-�� �̰ų� ��-�������� Ȯ���ϸ� ��.
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
		// ȸ�� �ʿ� ����.
		UTexture2D* tex = Helper::SyncLoadResource<UTexture2D>(TEXT("Texture2D'/Game/Resources/Widgets/Images/four_t.four_t'"));
		//UTexture2D* tex = Four.LoadSynchronous();
		PipeImage->SetBrushFromTexture(tex);
		RotationInfo = 0u;
	}

	PipeImage->SetBrushFromTexture(Two.Get());
}
void UPipeGame_Node::OnClickedPipeButton()
{
	// ���� ȸ������ �������� ȸ�� �ִϸ��̼� ���
	PlayAnimation(AnimArray[RotationInfo]);
	// ���� ������ ȸ��.
	PipeNodeRef.RotatePipe();
	// ȸ���� ����
	RotationInfo += RotationInfo == 3 ? -3 : 1;
}
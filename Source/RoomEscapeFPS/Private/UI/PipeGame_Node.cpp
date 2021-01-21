// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PipeGame_Node.h"
#include "GameFramework/RoomEscapeFPSGameMode.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Helper/Helper.h"

//#define STRAIGHT_TWO_PATH TEXT("Texture2D'/Game/Resources/Widgets/Images/two_straight_t.two_straight_t'")

void UPipeGame_Node::InitializePipeNode(FPipeNode* InNode)
{
	PipeNodeRef = InNode;
	// ȸ�� ��ư ���ε�
	PipeButton->OnClicked.AddDynamic(this, &UPipeGame_Node::OnClickedPipeButton);

	// ���۵� ��� ������ �������� �̹��� ���� �� ȸ��
	if (PipeNodeRef)
	{
		EPipeType type = PipeNodeRef->GetPipeType();
		if (type == EPipeType::EStraight_Two)
		{
			//// �񵿱� �ε�.
			//auto AfterLoadImage = [&]() 
			//{
			//	TAssetPtr<UTexture2D> texture(Straight_Two);
			//	if (texture.IsValid())
			//	{
			//		PipeImage->SetBrushFromTexture(texture.Get());
			//	}
			//};
			//Helper::AsyncLoadResource(UITextureAssets[0], AfterLoadImage);
			
			// ���� �ؽ��� �����̹Ƿ� ���� �ε�
			//PipeImage->SetBrushFromTexture(Straight_Two.Get());
			
			if (UITextureAssets[0].IsValid())
			{
				UTexture2D* tex = Helper::SyncLoadResourceByString<UTexture2D>(UITextureAssets[0].GetAssetPathString());
				PipeImage->SetBrushFromTexture(tex);
			}
			
		}

		
	}
}
void UPipeGame_Node::OnClickedPipeButton()
{
	// ���� ȸ������ �������� ȸ�� �ִϸ��̼� ���
	// ���� ������ ȸ��.
}
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
	// 회전 버튼 바인딩
	PipeButton->OnClicked.AddDynamic(this, &UPipeGame_Node::OnClickedPipeButton);

	// 전송된 노드 데이터 기준으로 이미재 생성 및 회전
	if (PipeNodeRef)
	{
		EPipeType type = PipeNodeRef->GetPipeType();
		if (type == EPipeType::EStraight_Two)
		{
			//// 비동기 로딩.
			//auto AfterLoadImage = [&]() 
			//{
			//	TAssetPtr<UTexture2D> texture(Straight_Two);
			//	if (texture.IsValid())
			//	{
			//		PipeImage->SetBrushFromTexture(texture.Get());
			//	}
			//};
			//Helper::AsyncLoadResource(UITextureAssets[0], AfterLoadImage);
			
			// 작은 텍스쳐 한장이므로 동기 로딩
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
	// 현재 회전값을 기준으로 회전 애니메이션 출력
	// 원본 데이터 회전.
}
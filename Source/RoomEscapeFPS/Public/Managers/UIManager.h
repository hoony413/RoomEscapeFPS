// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Managers/BaseManager.h"
#include "UI/BaseWidget.h"
#include "UIManager.generated.h"

/**
 * UI를 편하게 생성/호출하기 위한 매니저 클래스.
 */
UCLASS()
class ROOMESCAPEFPS_API UUIManager : public UBaseManager
{
	GENERATED_BODY()
	
public:
	virtual void InitManager() override;
	virtual void ReleaseManager() override;
	
	template<typename T>
	T* GetWidget()
	{
		UWorld* world = GetWorld();
		check(world);

		// UI매니져가 관리가능한 클래스인지 확인(UBaseWidget의 자식인지)
		check(T::StaticClass()->IsChildOf(UBaseWidget::StaticClass()));

		// CDO로부터 BP경로 받기
		FString path = Cast<UBaseWidget>(T::StaticClass()->ClassDefaultObject)->GetBPPath();
		check(!path.IsEmpty());

		// 오브젝트 로드(위젯BP)
		UClass* classObj = LoadObject<UClass>(world, *path);
		check(classObj);

		// 위젯 생성
		T* widget = Cast<T>(UUserWidget::CreateWidgetInstance(*world, classObj, NAME_None));
		check(widget);

		return widget;
	}

	template<typename T>
	T* OpenWidget()
	{
		T* t = GetWidget<T>();
		if (t == nullptr)
			return nullptr;

		if (t->IsInViewport() == false)
		{
			t->AddToPlayerScreen();
		}
		else
		{
			t->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		return t;
	}
};

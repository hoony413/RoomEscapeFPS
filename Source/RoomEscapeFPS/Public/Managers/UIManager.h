// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Managers/BaseManager.h"
#include "UI/BaseWidget.h"
#include "UIManager.generated.h"

/**
 같은 타입으로 이미 인스턴스 된게 있으면 안의 내용만 바꿔준다.
 없으면 새로 만들어서 뷰포트에 올려준다.
 UI오브젝트의 생명주기를 관리한다.
 상태는 3가지로 관리한다
 인스턴스되지 않음
 인스턴스됨 + 화면에 없음(숨김처리)
 인스턴스됨 + 화면에 있음(표시 중)
 */
UCLASS()
class ROOMESCAPEFPS_API UUIManager : public UBaseManager
{
	GENERATED_BODY()
	
public:
	virtual void InitManager() override;
	virtual void ReleaseManager() override;
	
	template<typename T>
	T* GetWidget(bool bAllowReduplicate = false)
	{
		if (Widgets.Contains(T::StaticClass()->GetFName()) == false)
		{
			// UI매니져가 관리가능한 클래스인지 확인(UBaseWidget의 자식인지)
			check(T::StaticClass()->IsChildOf(UBaseWidget::StaticClass()));

			// CDO로부터 BP경로 받기
			FString path = Cast<UBaseWidget>(T::StaticClass()->ClassDefaultObject)->GetBPPath();
			
			// 오브젝트 로드(위젯BP)
			UClass* classObj = LoadObject<UClass>(GetWorld(), *path);
			check(classObj != nullptr);
			
			// 위젯 생성
			T* widget = Cast<T>(UUserWidget::CreateWidgetInstance(*(GetWorld()), 
				classObj, T::StaticClass()->GetFName()));
			if (!bAllowReduplicate)
			{	// 중복 미허용인 경우에만 관리대상으로 추가한다.
				// 중복 허용: 화면에 2개를 띄운다는 의미: 관리할 필요가 없음.
				Widgets.Add(T::StaticClass()->GetFName(), widget);
			}
			return widget;
		}
		// 이미 있는 경우(동일한 위젯BP를 사용한 적 있는 경우) 그것을 리턴해준다.
		return Cast<T>(Widgets[T::StaticClass()->GetFName()]);
	}

	template<typename T>
	T* ShowWidget(bool bAllowReduplicate = false)
	{
		T* t = GetWidget<T>(bAllowReduplicate);
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

	template<typename T>
	void RemoveWidget()
	{
		if (Widgets.Contains(T::StaticClass()->GetFName()) == false)
			return;

		Widgets[T::StaticClass()->GetFName()]->RemoveFromParent();
		Widgets.Remove(T::StaticClass()->GetFName());
	}

private:
	UPROPERTY()
	TMap<FName, class UBaseWidget*> Widgets;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Managers/BaseManager.h"
#include "UI/BaseWidget.h"
#include "UIManager.generated.h"

/**
 * UI�� ���ϰ� ����/ȣ���ϱ� ���� �Ŵ��� Ŭ����.
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

		// UI�Ŵ����� ���������� Ŭ�������� Ȯ��(UBaseWidget�� �ڽ�����)
		check(T::StaticClass()->IsChildOf(UBaseWidget::StaticClass()));

		// CDO�κ��� BP��� �ޱ�
		FString path = Cast<UBaseWidget>(T::StaticClass()->ClassDefaultObject)->GetBPPath();
		check(!path.IsEmpty());

		// ������Ʈ �ε�(����BP)
		UClass* classObj = LoadObject<UClass>(world, *path);
		check(classObj);

		// ���� ����
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

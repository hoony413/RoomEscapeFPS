// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Managers/BaseManager.h"
#include "UI/BaseWidget.h"
#include "UIManager.generated.h"

/**
 ���� Ÿ������ �̹� �ν��Ͻ� �Ȱ� ������ ���� ���븸 �ٲ��ش�.
 ������ ���� ���� ����Ʈ�� �÷��ش�.
 UI������Ʈ�� �����ֱ⸦ �����Ѵ�.
 ���´� 3������ �����Ѵ�
 �ν��Ͻ����� ����
 �ν��Ͻ��� + ȭ�鿡 ����(����ó��)
 �ν��Ͻ��� + ȭ�鿡 ����(ǥ�� ��)
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
			// UI�Ŵ����� ���������� Ŭ�������� Ȯ��(UBaseWidget�� �ڽ�����)
			check(T::StaticClass()->IsChildOf(UBaseWidget::StaticClass()));

			// CDO�κ��� BP��� �ޱ�
			FString path = Cast<UBaseWidget>(T::StaticClass()->ClassDefaultObject)->GetBPPath();
			
			// ������Ʈ �ε�(����BP)
			UClass* classObj = LoadObject<UClass>(GetWorld(), *path);
			check(classObj != nullptr);
			
			// ���� ����
			T* widget = Cast<T>(UUserWidget::CreateWidgetInstance(*(GetWorld()), 
				classObj, T::StaticClass()->GetFName()));
			if (!bAllowReduplicate)
			{	// �ߺ� ������� ��쿡�� ����������� �߰��Ѵ�.
				// �ߺ� ���: ȭ�鿡 2���� ���ٴ� �ǹ�: ������ �ʿ䰡 ����.
				Widgets.Add(T::StaticClass()->GetFName(), widget);
			}
			return widget;
		}
		// �̹� �ִ� ���(������ ����BP�� ����� �� �ִ� ���) �װ��� �������ش�.
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

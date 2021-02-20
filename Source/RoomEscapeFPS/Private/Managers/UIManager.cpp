// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/UIManager.h"
//#include "D:\UnrealEngine\Engine\Source\Editor\UnrealEd\Public\Kismet2\KismetEditorUtilities.h"
#include "UI/BaseWidget.h"

void UUIManager::InitManager()
{
	Super::InitManager();
////backup code
//	for (TObjectIterator<UClass> ClassIt; ClassIt; ++ClassIt)
//	{
//		UClass* Class = *ClassIt;
//
//		// Only interested in native C++ classes
//		if (!Class->IsNative())
//		{
//			continue;
//		}
//
//		// Ignore deprecated
//		if (Class->HasAnyClassFlags(CLASS_Deprecated | CLASS_NewerVersionExists))
//		{
//			continue;
//		}
//
////#if WITH_EDITOR
////		// Ignore skeleton classes (semi-compiled versions that only exist in-editor)
////		if (FKismetEditorUtilities::IsClassABlueprintSkeleton(Class))
////		{
////			continue;
////		}
////#endif
//
//		// Check this class is a subclass of Base
//		if (!Class->IsChildOf(UBaseWidget::StaticClass()))
//		{
//			continue;
//		}
//
//		UBaseWidget* widgetCDO = Cast<UBaseWidget>(Class->ClassDefaultObject);
//		FString path = widgetCDO->GetBPPath();
//		if (!WidgetBPPathInfo.Contains(widgetCDO->GetFName()))
//		{
//			WidgetBPPathInfo.Add(widgetCDO->GetFName(), path);
//		}
//	}
}

//class UWidgetAnimation* UUIManager::GetWidgetAnimation(const FString& InAnimName)
//{
//	UProperty* prop = GetClass()->PropertyLink;
//	while (prop != nullptr)
//	{
//		if (prop->GetClass() == UObjectProperty::StaticClass())
//		{
//			UObjectProperty* objectProp = Cast<UObjectProperty>(prop);
//
//			if (objectProp->PropertyClass == UWidgetAnimation::StaticClass())
//			{
//				UWidgetAnimation* widgetAnim = Cast<UWidgetAnimation>(objectProp->GetObjectPropertyValue_InContainer(this));
//				if (widgetAnim != nullptr && widgetAnim->GetMovieScene()->GetName().Compare(InAnimName, ESearchCase::IgnoreCase) == 0)
//				{
//					return widgetAnim;
//				}
//			}
//		}
//		prop = prop->PropertyLinkNext;
//	}
//}

void UUIManager::ReleaseManager()
{
	Super::ReleaseManager();
}
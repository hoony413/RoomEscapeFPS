// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/UIManager.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "UI/BaseWidget.h"

void UUIManager::InitManager()
{
	Super::InitManager();
//	
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
//#if WITH_EDITOR
//		// Ignore skeleton classes (semi-compiled versions that only exist in-editor)
//		if (FKismetEditorUtilities::IsClassABlueprintSkeleton(Class))
//		{
//			continue;
//		}
//#endif
//
//		// Check this class is a subclass of Base
//		if (!Class->IsChildOf(UBaseWidget::StaticClass()))
//		{
//			continue;
//		}
//	}
}
void UUIManager::ReleaseManager()
{
	Widgets.Empty();
	Super::ReleaseManager();
}
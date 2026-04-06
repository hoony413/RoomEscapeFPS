// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "UI/LoadingScreen.h"
#include "UISettings.generated.h"

class UDataTable;
class UBaseHUDWidget;

/**
 * 에디터 Project Settings > RoomEscapeFPS > UI Settings 에서 설정한다.
 */
UCLASS(Config=RoomEscapeFPS, defaultconfig, meta=(DisplayName="UI Settings"))
class ROOMESCAPEFPS_API UUISettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	static const UUISettings* Get() { return GetDefault<UUISettings>(); }

	virtual FName GetCategoryName() const override { return TEXT("RoomEscapeFPS"); }
	virtual FName GetSectionName() const override { return TEXT("UISettings"); }

	// FActivatableWidgetInfo row 타입의 DataTable. 위젯 타입과 BP 클래스를 매핑한다.
	UPROPERTY(Config, EditAnywhere, Category="Widgets")
TSoftObjectPtr<UDataTable> WidgetDataTable;

	// 로딩 스크린은 DataTable 외부에서 별도 관리한다. (GameInstance outer 필요)
	UPROPERTY(Config, EditAnywhere, Category="Loading")
TSoftClassPtr<ULoadingScreen> LoadingWidgetClass;

	// BaseHUDWidget BP 클래스. ARoomEscapeFPSHUD::InitializeHUD 시 생성한다.
	UPROPERTY(Config, EditAnywhere, Category="Widgets")
TSoftClassPtr<UBaseHUDWidget> HUDWidgetClass;
};

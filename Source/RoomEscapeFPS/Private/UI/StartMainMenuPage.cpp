// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/StartMainMenuPage.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "GameFramework/GameUserSettings.h"
#include "Managers/RoomEscapeFPSGameInstance.h"
#include "GameFramework/RoomEscapeFPSHUD.h"
#if WITH_EDITOR
#include "Kismet/KismetSystemLibrary.h"
#endif

void UStartMainMenuPage::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (StartButton && StartButton->OnClicked.IsBound() == false)
	{
		StartButton->OnClicked.AddDynamic(this, &UStartMainMenuPage::OnClickedStartButton);
	}
	if (OptionButton && OptionButton->OnClicked.IsBound() == false)
	{
		OptionButton->OnClicked.AddDynamic(this, &UStartMainMenuPage::OnClickedOptionButton);
	}
	if (ExitButton && ExitButton->OnClicked.IsBound() == false)
	{
		ExitButton->OnClicked.AddDynamic(this, &UStartMainMenuPage::OnClickedExitButton);
	}

	GetOwningPlayer()->SetShowMouseCursor(true);
	PlayAnimation(AnimArray[0]);
}

void UStartMainMenuPage::OnClickedStartButton()
{
	// TODO: 로딩 화면으로 전환 / 레벨 전환 / 접속 완료 시 화면 표시.
	UGameplayStatics::OpenLevel(this, TEXT("127.0.0.1"));
	GetOwningPlayer()->SetShowMouseCursor(false);

	ARoomEscapeFPSHUD* hud = Cast<ARoomEscapeFPSHUD>(GetOwningPlayer()->GetHUD());
	if (hud)
	{
		hud->SetVisibilityLoadingScreen(true);
	}
	RemoveFromParent();
}
void UStartMainMenuPage::OnClickedOptionButton()
{
	// TODO: 창모드 / 밝기 / 소리 조절 따위의 옵션 UI 생성(우선순위 낮음)
	// 아래 식으로 해상도 조절 가능.
	//UGameUserSettings* MyGameSettings = GEngine->GetGameUserSettings();
	//MyGameSettings->SetScreenResolution(FIntPoint(1024, 768));
	//MyGameSettings->SetFullscreenMode(EWindowMode::Windowed);
}
void UStartMainMenuPage::OnClickedExitButton()
{
	// TODO: 게임 끄기.
	URoomEscapeFPSGameInstance* gi = Cast<URoomEscapeFPSGameInstance>(GetWorld()->GetGameInstance());
	gi->Shutdown();
	
#if WITH_EDITOR
	UKismetSystemLibrary::QuitGame(GetWorld(), UGameplayStatics::GetPlayerController(GetWorld(), 0), EQuitPreference::Quit, false);
#else
	FPlatformMisc::RequestExit(false); 
#endif
}
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
	// TODO: �ε� ȭ������ ��ȯ / ���� ��ȯ / ���� �Ϸ� �� ȭ�� ǥ��.
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
	// TODO: â��� / ��� / �Ҹ� ���� ������ �ɼ� UI ����(�켱���� ����)
	// �Ʒ� ������ �ػ� ���� ����.
	//UGameUserSettings* MyGameSettings = GEngine->GetGameUserSettings();
	//MyGameSettings->SetScreenResolution(FIntPoint(1024, 768));
	//MyGameSettings->SetFullscreenMode(EWindowMode::Windowed);
}
void UStartMainMenuPage::OnClickedExitButton()
{
	// TODO: ���� ����.
	URoomEscapeFPSGameInstance* gi = Cast<URoomEscapeFPSGameInstance>(GetWorld()->GetGameInstance());
	gi->Shutdown();
	
#if WITH_EDITOR
	UKismetSystemLibrary::QuitGame(GetWorld(), UGameplayStatics::GetPlayerController(GetWorld(), 0), EQuitPreference::Quit, false);
#else
	FPlatformMisc::RequestExit(false); 
#endif
}
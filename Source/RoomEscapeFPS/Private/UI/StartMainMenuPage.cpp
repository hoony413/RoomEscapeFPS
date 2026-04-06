// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/StartMainMenuPage.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Helper/Helper.h"
#include "Managers/UISubsystem.h"
#if WITH_EDITOR
#endif

void UStartMainMenuPage::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (StartButton && not StartButton->OnClicked.IsBound())
	{
		StartButton->OnClicked.AddDynamic(this, &UStartMainMenuPage::OnClickedStartButton);
	}
	if (OptionButton && not OptionButton->OnClicked.IsBound())
	{
		OptionButton->OnClicked.AddDynamic(this, &UStartMainMenuPage::OnClickedOptionButton);
	}
	if (ExitButton && not ExitButton->OnClicked.IsBound())
	{
		ExitButton->OnClicked.AddDynamic(this, &UStartMainMenuPage::OnClickedExitButton);
	}

	GetOwningPlayer()->SetShowMouseCursor(true);
	PlayAnimation(AnimArray[0]);
}

void UStartMainMenuPage::OnClickedStartButton()
{
	UUISubsystem* uiSubsystem = Helper::GetSubsystem<UUISubsystem>(GetWorld());
	if (uiSubsystem)
	{
		uiSubsystem->ShowLoadingScreen();
	}

	GetOwningPlayer()->SetShowMouseCursor(false);
	UGameplayStatics::OpenLevel(this, TEXT("127.0.0.1"));
	RemoveFromParent();
}
void UStartMainMenuPage::OnClickedOptionButton()
{
}
void UStartMainMenuPage::OnClickedExitButton()
{
#if WITH_EDITOR
	UKismetSystemLibrary::QuitGame(GetWorld(), UGameplayStatics::GetPlayerController(GetWorld(), 0), EQuitPreference::Quit, false);
#else
	FPlatformMisc::RequestExit(false); 
#endif
}
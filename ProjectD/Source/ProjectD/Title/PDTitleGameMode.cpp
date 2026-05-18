// Fill out your copyright notice in the Description page of Project Settings.


#include "PDTitleGameMode.h"

#include "../UI/Title/PDTitleMainWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "../UI/Core/PDUIManagerSubsystem.h"
#include "PDDefine.h"
#include "../Sound/PDSoundManagerSubsystem.h"

void APDTitleGameMode::BeginPlay()
{
	Super::BeginPlay();

	SetupTitleUI();
	SetupEnhancedInput();

	if (UGameInstance* GI = GetWorld()->GetGameInstance())
	{
		if (UPDSoundManagerSubsystem* SoundMgr = GI->GetSubsystem<UPDSoundManagerSubsystem>())
		{
			SoundMgr->PlayBGMTitle();
		}
	}
}

void APDTitleGameMode::SetupTitleUI()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	APlayerController* PC = World->GetFirstPlayerController();
	UGameInstance* GI = World->GetGameInstance();
	UPDUIManagerSubsystem* UIManager = GI ? GI->GetSubsystem<UPDUIManagerSubsystem>() : nullptr;

	if (!PC || !GI || !UIManager)
	{
		return;
	}

	PC->bShowMouseCursor = true;
	PC->SetInputMode(FInputModeGameAndUI());

	UIManager->ClearLayer(EUILayer::Screen);

	TitleWidget = Cast<UPDTitleMainWidget>(UIManager->AddWidget(EUIType::TitleMain));
	if (TitleWidget)
	{
		TitleWidget->OnStartRequested.AddUObject(this, &APDTitleGameMode::HandleStartRequestedFromWidget);
	}
}

void APDTitleGameMode::SetupEnhancedInput()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;
	if (ULocalPlayer* LP = PC->GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (TitleMappingContext)
			{
				Subsystem->AddMappingContext(TitleMappingContext, 0);
			}
		}
	}
	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PC->InputComponent))
	{
		if (TapAction)
		{
			EIC->BindAction(TapAction, ETriggerEvent::Started, this, &APDTitleGameMode::HandleTapInput);
		}
	}
}

void APDTitleGameMode::StartBattleLevel()
{
	if (UGameInstance* GI = GetWorld()->GetGameInstance())
	{
		if (UPDUIManagerSubsystem* UIManager = GI->GetSubsystem<UPDUIManagerSubsystem>())
		{
			UIManager->ClearLayer(EUILayer::Screen);
			UIManager->ClearLayer(EUILayer::Popup);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("[PD][APDTitleGameMode][StartBattleLevel]"));

	UGameplayStatics::OpenLevel(this, BattleLevelName);
}

void APDTitleGameMode::HandleTapInput(const FInputActionValue& Value)
{
	StartBattleLevel();
}

void APDTitleGameMode::HandleStartRequestedFromWidget()
{
	StartBattleLevel();
}

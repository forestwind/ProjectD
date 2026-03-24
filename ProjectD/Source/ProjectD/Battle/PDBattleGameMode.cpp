// Fill out your copyright notice in the Description page of Project Settings.


#include "PDBattleGameMode.h"

#include "Blueprint/UserWidget.h"
#include "Manager/ModelManager.h"
#include "Manager/PDStageRoundSpawner.h"
#include "Character/PDCharacter.h"
#include "UI/Core/PDUIManagerSubsystem.h"
#include "Table/PDTableManagerSubsystem.h"
#include "Player/PDPlayerController.h"
#include "Kismet/GameplayStatics.h"

APDBattleGameMode::APDBattleGameMode()
{
	GameStateType = EGameState::Ready;
	StageID = 1;
}

void APDBattleGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	Spawner = NewObject<UPDStageRoundSpawner>(this);
	if (!Spawner)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PD][BattleGameMode][InitGame] Spawner is null."));
		return;
	}
	Spawner->Initialize(ModelManager);
}

void APDBattleGameMode::BeginPlay()
{
	Super::BeginPlay();

	ReadyGame();

	// game state test
	FTimerHandle StartTimer;
	GetWorld()->GetTimerManager().SetTimer(StartTimer, this, &APDBattleGameMode::StartGame, 3.0f, false);
}

void APDBattleGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

void APDBattleGameMode::RestartPlayer(AController* NewPlayer)
{
	if (APDPlayerController* PDPlayerController = Cast<APDPlayerController>(NewPlayer))
	{
		UPDTableManagerSubsystem* TableManager = UGameInstance::GetSubsystem<UPDTableManagerSubsystem>(UGameplayStatics::GetGameInstance(this));
		if (TableManager == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Invalid TableManager"));
			return;
		}

		DefaultPawnClass = TableManager->GetUnitBP(PDPlayerController->GetPlayerUnitID());
		if (DefaultPawnClass == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("[PD][BattleGameMode][RestartPlayer] Invalid DefaultPawnClass!!"));
		}
	}
	
	Super::RestartPlayer(NewPlayer);
}

void APDBattleGameMode::DespawnUnit(const FGuid& InUnitGuid)
{
	if (ModelManager)
	{
		ModelManager->DespawnCharacter(InUnitGuid);
	}
}

void APDBattleGameMode::ChangeGameState(const EGameState InGameState)
{
	GameStateType = InGameState;
}

void APDBattleGameMode::ReadyGame()
{
	ChangeGameState(EGameState::Ready);
	StartStage(StageID);
}

const TCHAR* APDBattleGameMode::BattleMainWidgetPath = TEXT("/Game/UI/Battle/WBP_BattleMainUI.WBP_BattleMainUI_C");

void APDBattleGameMode::StartGame()
{
	ChangeGameState(EGameState::Play);
	UE_LOG(LogTemp, Warning, TEXT("[PD][BattleGameMode][StartGame] Game Start!!"));

	// WBP_BattleMainUI 경로로 로드 후 UIManager Panel2D에 추가
	UGameInstance* GI = GetWorld()->GetGameInstance();
	UPDUIManagerSubsystem* UIManager = GI ? GI->GetSubsystem<UPDUIManagerSubsystem>() : nullptr;
	if (UIManager)
	{
		if (UClass* WidgetClass = LoadClass<UPDUIBattleMainWidget>(nullptr, BattleMainWidgetPath))
		{
			BattleMainWidget = CreateWidget<UPDUIBattleMainWidget>(GI, WidgetClass);
			if (BattleMainWidget)
			{
				UIManager->AddWidgetToPanel2D(BattleMainWidget);
			}
		}
	}
}

void APDBattleGameMode::EndGame()
{
	ChangeGameState(EGameState::End);

	UE_LOG(LogTemp, Warning, TEXT("[PD][BattleGameMode][EndGame] Game Clear!!"));
}

void APDBattleGameMode::StartStage(const int32 InStageID)
{
	StageID = InStageID;
}

void APDBattleGameMode::SpawnStageUnit()
{
	if (!ModelManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PD][BattleGameMode][SpawnStageUnit] ModelManager is null."));
		return;
	}

	if (!Spawner)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PD][BattleGameMode][InitGame] Spawner is null."));
		return;
	}

	// ===== Enemy Spawn =====

}

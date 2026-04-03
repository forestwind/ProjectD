// Fill out your copyright notice in the Description page of Project Settings.


#include "PDBattleGameMode.h"

#include "Blueprint/UserWidget.h"
#include "UI/Battle/PDUIBattleMainWidget.h"
#include "UI/Battle/PDUIBattleEndWidget.h"
#include "UI/Battle/PDUIBattlePhaseMsgWidget.h"
#include "Manager/ModelManager.h"
#include "Battle/PDBattleSpawnActor.h"
#include "Character/PDCharacter.h"
#include "UI/Core/PDUIManagerSubsystem.h"
#include "Table/PDTableManagerSubsystem.h"
#include "Sound/PDSoundManagerSubsystem.h"
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
}

void APDBattleGameMode::BeginPlay()
{
	Super::BeginPlay();

	ReadyGame();

	
	// game state test
	FTimerHandle StartTimer;
	GetWorld()->GetTimerManager().SetTimer(StartTimer, this, &APDBattleGameMode::StartGame, ReadyMessageDuration, false);
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
		RemainingEnemyCount = FMath::Max(0, RemainingEnemyCount - 1);
		UpdateMonsterCount();
	}
}

void APDBattleGameMode::UpdateMonsterCount()
{
	if (BattleMainWidget)
	{
		BattleMainWidget->UpdateMonsterCount(RemainingEnemyCount, TotalEnemyCount);
	}
}

void APDBattleGameMode::UpdatePlayerHP(int32 CurHP, int32 MaxHP)
{
	if (BattleMainWidget)
	{
		BattleMainWidget->UpdatePlayerHP(CurHP, MaxHP);
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
	SpawnStageUnit();
	ShowUIPhaseMessage(EGameState::Ready);

	UGameInstance* GI = GetWorld()->GetGameInstance();
	if (UPDSoundManagerSubsystem* SoundMgr = GI ? GI->GetSubsystem<UPDSoundManagerSubsystem>() : nullptr)
	{
		SoundMgr->PlayBGMBattle();
	}
}

void APDBattleGameMode::StartGame()
{
	ChangeGameState(EGameState::Play);
	UE_LOG(LogTemp, Warning, TEXT("[PD][BattleGameMode][StartGame] Game Start!!"));
	ShowUIPhaseMessage(EGameState::Play);

	UGameInstance* GI = GetWorld()->GetGameInstance();
	UPDUIManagerSubsystem* UIManager = GI ? GI->GetSubsystem<UPDUIManagerSubsystem>() : nullptr;
	if (UIManager && BattleMainWidgetClass)
	{
		BattleMainWidget = CreateWidget<UPDUIBattleMainWidget>(GI, BattleMainWidgetClass);
		if (BattleMainWidget)
		{
			UIManager->AddWidgetToPanel2D(BattleMainWidget);
			UpdateMonsterCount();
		}
	}
}

void APDBattleGameMode::EndGame()
{
	ChangeGameState(EGameState::End);
	
	UE_LOG(LogTemp, Warning, TEXT("[PD][BattleGameMode][EndGame] Game Clear!!"));
	ShowBattleEndUI();
}

void APDBattleGameMode::ShowBattleEndUI()
{
	UGameInstance* GI = GetWorld()->GetGameInstance();
	UPDUIManagerSubsystem* UIManager = GI ? GI->GetSubsystem<UPDUIManagerSubsystem>() : nullptr;
	if (!UIManager)
	{
		return;
	}

	if (BattleMainWidget)
	{
		UIManager->RemoveWidgetFromPanel2D(BattleMainWidget);
		BattleMainWidget = nullptr;
	}

	if (BattleEndWidgetClass)
	{
		BattleEndWidget = CreateWidget<UPDUIBattleEndWidget>(GI, BattleEndWidgetClass);
		if (BattleEndWidget)
		{
			const FText ResultText = (RemainingEnemyCount == 0)
				? FText::FromString(TEXT("Victory"))
				: FText::FromString(TEXT("Defeat"));
			BattleEndWidget->SetResultText(ResultText);
			UIManager->AddWidgetToPanel2D(BattleEndWidget);
		}
	}
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

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APDBattleSpawnActor::StaticClass(), FoundActors);

	UE_LOG(LogTemp, Log, TEXT("[PD][BattleGameMode][SpawnStageUnit] Found SpawnActor Count: %d"), FoundActors.Num());

	for (AActor* FoundActor : FoundActors)
	{
		const APDBattleSpawnActor* SpawnActor = Cast<APDBattleSpawnActor>(FoundActor);
		if (!SpawnActor)
		{
			continue;
		}

		const TCHAR* SpawnTypeText = TEXT("None");
		switch (SpawnActor->SpawnType)
		{
		case EPDBattleSpawnType::Enemy:
			if (ModelManager->SpawnCharacter(SpawnActor->SpawnTypeID, SpawnActor->SpawnTypeLevel, SpawnActor->GetActorLocation(), SpawnActor->GetActorRotation()))
			{
				++TotalEnemyCount;
				++RemainingEnemyCount;
			}
			break;
		case EPDBattleSpawnType::Item:
			break;
		default:
			break;
		}

		UE_LOG(
			LogTemp,
			Log,
			TEXT("[PD][BattleGameMode][SpawnStageUnit] SpawnActor - ID:%d Type:%s TypeID:%d Location:%s Rotation:%s"),
			SpawnActor->BattleSpawnID,
			*UEnum::GetDisplayValueAsText(SpawnActor->SpawnType).ToString(),
			SpawnActor->SpawnTypeID,
			*SpawnActor->GetActorLocation().ToString(),
			*SpawnActor->GetActorRotation().ToString()
		);
	}
}

void APDBattleGameMode::ShowPhaseMessage(const FText& InText)
{
	UGameInstance* GI = GetWorld() ? GetWorld()->GetGameInstance() : nullptr;
	if (!GI)
	{
		return;
	}
	
	UPDUIManagerSubsystem* UIManager = GI->GetSubsystem<UPDUIManagerSubsystem>();
	if (!UIManager)
	{
		return;
	}
	
	// 위젯 생성(처음 1회)
	if (!PhaseMsgWidget && PhaseMsgWidgetClass)
	{
		PhaseMsgWidget = CreateWidget<UPDUIBattlePhaseMsgWidget>(GI, PhaseMsgWidgetClass);
	}
	if (!PhaseMsgWidget)
	{
		return;
	}
	
	PhaseMsgWidget->SetPhaseText(InText);
	UIManager->AddWidgetToPanelOverlay(PhaseMsgWidget);
	
}

void APDBattleGameMode::HidePhaseMessage()
{
	if (UGameInstance* GI = GetWorld() ? GetWorld()->GetGameInstance() : nullptr)
	{
		if (UPDUIManagerSubsystem* UIManager = GI->GetSubsystem<UPDUIManagerSubsystem>())
		{
			UIManager->RemoveWidgetFromPanelOverlay(PhaseMsgWidget);
		}
	}
}

void APDBattleGameMode::ShowUIPhaseMessage(EGameState state)
{
	if (state == EGameState::Ready)
	{
		// READY 메시지 표시(Overlay)
		if (UGameInstance* GI = GetWorld() ? GetWorld()->GetGameInstance() : nullptr)
		{
			if (UPDUIManagerSubsystem* UIManager = GI->GetSubsystem<UPDUIManagerSubsystem>())
			{
				UIManager->RemoveWidgetFromPanelOverlay(PhaseMsgWidget);
				ShowPhaseMessage(FText::FromString(TEXT("READY")));
			}
		}
	}
	else if (state == EGameState::Play)
	{
		// START 메시지 표시(Overlay)
		if (UGameInstance* GI = GetWorld() ? GetWorld()->GetGameInstance() : nullptr)
		{
			if (UPDUIManagerSubsystem* UIManager = GI->GetSubsystem<UPDUIManagerSubsystem>())
			{
				UIManager->RemoveWidgetFromPanelOverlay(PhaseMsgWidget);
				ShowPhaseMessage(FText::FromString(TEXT("START")));
			}
		}
	
		if (StartMessageDuration > 0.0f)
		{
			GetWorld()->GetTimerManager().SetTimer(
				HidePhaseMsgTimerHandle,
				this,
				&APDBattleGameMode::HidePhaseMessage,
				StartMessageDuration,
				false
			);
		}
		else
		{
			HidePhaseMessage();
		}
	}
}

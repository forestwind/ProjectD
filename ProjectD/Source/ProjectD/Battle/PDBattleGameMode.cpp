// Fill out your copyright notice in the Description page of Project Settings.


#include "PDBattleGameMode.h"

#include "Blueprint/UserWidget.h"
#include "UI/Battle/PDUIBattleMainWidget.h"
#include "UI/Battle/PDUIBattleEndWidget.h"
#include "UI/Battle/PDUIBattlePhaseMsgWidget.h"
#include "UI/Battle/PDUIBattleInventoryWidget.h"
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

void APDBattleGameMode::SetPlayerUnitGuid(const FGuid& InUnitGuid)
{
	PlayerUnitGuid = InUnitGuid;
}

void APDBattleGameMode::DespawnUnit(const FGuid& InUnitGuid)
{
	if (ModelManager == nullptr)
	{
		return;
	}

	ModelManager->DespawnCharacter(InUnitGuid);
	if (InUnitGuid == PlayerUnitGuid)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PD][BattleGameMode][DespawnUnit] Player Unit Died!!"));
		DefeatGame();
	}
	else
	{
		RemainingEnemyCount = FMath::Max(0, RemainingEnemyCount - 1);
		UpdateMonsterCount();
		if (RemainingEnemyCount == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("[PD][BattleGameMode][DespawnUnit] All Monster Died!!"));
			VictoryGame();
		}
	}
}

void APDBattleGameMode::DespawnItem(const FGuid& InItemGuid)
{
	if (ModelManager)
	{
		ModelManager->DespawnBattleItem(InItemGuid);
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
	BattleInventory = NewObject<UPDBattleInventory>(this, TEXT("BattleInventory"));

	if (UGameInstance* GI = GetGameInstance())
	{
		if (UPDTableManagerSubsystem* TableManager = GI->GetSubsystem<UPDTableManagerSubsystem>())
		{
			BattleInventory->Initialize(TableManager, 32);
			// 더미 아이템 기본 지급
			BattleInventory->AddItem(1, 1);
		}
	}

	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		PC->SetInputMode(FInputModeUIOnly());
	}

	ChangeGameState(EGameState::Ready);
	StartStage(StageID);
	SpawnStageUnit();
	ShowUIPhaseMessage(EGameState::Ready);

	CreateUI();

	UGameInstance* GI = GetWorld()->GetGameInstance();
	if (UPDSoundManagerSubsystem* SoundMgr = GI ? GI->GetSubsystem<UPDSoundManagerSubsystem>() : nullptr)
	{
		SoundMgr->PlayBGMBattle();
	}
}

void APDBattleGameMode::StartGame()
{
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		PC->SetInputMode(FInputModeGameAndUI());
	}

	ChangeGameState(EGameState::Play);
	UE_LOG(LogTemp, Warning, TEXT("[PD][BattleGameMode][StartGame] Game Start!!"));
	ShowUIPhaseMessage(EGameState::Play);

	if (BattleMainWidget)
	{
		BattleMainWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		UpdateMonsterCount();

		if (ModelManager)
		{
			if (APDCharacter* PlayerChar = ModelManager->FindCharacter(PlayerUnitGuid))
			{
				UpdatePlayerHP(PlayerChar->GetCurHP(), PlayerChar->GetMaxHP());
			}
		}
	}

}

void APDBattleGameMode::VictoryGame()
{
	ChangeGameState(EGameState::Victory);

	UE_LOG(LogTemp, Warning, TEXT("[PD][BattleGameMode][EndGame] Game Clear!!"));
	ShowBattleEndUI();
}

void APDBattleGameMode::DefeatGame()
{
	ChangeGameState(EGameState::Defeat);

	UE_LOG(LogTemp, Warning, TEXT("[PD][BattleGameMode][EndGame] Game Defeat!!"));
	ShowBattleEndUI();
}

void APDBattleGameMode::ShowBattleEndUI()
{
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		PC->FlushPressedKeys();
		PC->bShowMouseCursor = true;
		PC->SetInputMode(FInputModeUIOnly());
	}

	UGameInstance* GI = GetWorld()->GetGameInstance();
	UPDUIManagerSubsystem* UIManager = GI ? GI->GetSubsystem<UPDUIManagerSubsystem>() : nullptr;
	if (!UIManager)
	{
		return;
	}

	if (BattleMainWidget)
	{
		UIManager->RemoveWidget(BattleMainWidget);
		BattleMainWidget = nullptr;
	}

	BattleEndWidget = Cast<UPDUIBattleEndWidget>(UIManager->AddWidget(EUIType::BattleEnd));
	if (BattleEndWidget)
	{
		const FText ResultText = (GameStateType == EGameState::Victory)
			? FText::FromString(TEXT("Victory"))
			: FText::FromString(TEXT("Defeat"));
		BattleEndWidget->SetResultText(ResultText);
	}
}

void APDBattleGameMode::StartStage(const int32 InStageID)
{
	StageID = InStageID;
}

void APDBattleGameMode::CreateUI()
{
	UGameInstance* GI = GetWorld()->GetGameInstance();
	UPDUIManagerSubsystem* UIManager = GI ? GI->GetSubsystem<UPDUIManagerSubsystem>() : nullptr;
	if (!UIManager)
	{
		return;
	}

	BattleMainWidget = Cast<UPDUIBattleMainWidget>(UIManager->AddWidget(EUIType::BattleMain));
	if (BattleMainWidget)
	{
		BattleMainWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	BattleInventoryWidget = Cast<UPDUIBattleInventoryWidget>(UIManager->AddWidget(EUIType::BattleInventory));
	if (BattleInventoryWidget)
	{
		BattleInventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
		bInventoryVisible = false;
	}
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
			ModelManager->SpawnBattleItem(SpawnActor->SpawnTypeID, SpawnActor->GetActorLocation(), SpawnActor->GetActorRotation());
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
	UPDUIManagerSubsystem* UIManager = GI ? GI->GetSubsystem<UPDUIManagerSubsystem>() : nullptr;
	if (!UIManager)
	{
		return;
	}

	PhaseMsgWidget = Cast<UPDUIBattlePhaseMsgWidget>(UIManager->AddWidget(EUIType::BattlePhaseMsg));
	if (PhaseMsgWidget)
	{
		PhaseMsgWidget->SetPhaseText(InText);
	}
}

void APDBattleGameMode::HidePhaseMessage()
{
	if (UGameInstance* GI = GetWorld() ? GetWorld()->GetGameInstance() : nullptr)
	{
		if (UPDUIManagerSubsystem* UIManager = GI->GetSubsystem<UPDUIManagerSubsystem>())
		{
			UIManager->RemoveWidget(PhaseMsgWidget);
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
				UIManager->RemoveWidget(PhaseMsgWidget);
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
				UIManager->RemoveWidget(PhaseMsgWidget);
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

void APDBattleGameMode::ToggleBattleInventory()
{
	if (!BattleInventoryWidget)
	{
		return;
	}

	bInventoryVisible = !bInventoryVisible;

	if (bInventoryVisible && BattleInventory)
	{
		BattleInventoryWidget->ClearItems();
		for (const FPDBattleInventorySlot& Slot : BattleInventory->GetSlots())
		{
			BattleInventoryWidget->AddItem(Slot);
		}
	}

	BattleInventoryWidget->SetVisibility(bInventoryVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

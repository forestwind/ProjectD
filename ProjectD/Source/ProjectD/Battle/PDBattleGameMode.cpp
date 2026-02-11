// Fill out your copyright notice in the Description page of Project Settings.


#include "PDBattleGameMode.h"

#include "Blueprint/UserWidget.h"
#include "../Manager/ModelManager.h"
#include "../Manager/PDStageRoundSpawner.h"
#include "../Character/PDCharacter.h"
#include "../UI/Core/PDUIManagerSubsystem.h"


APDBattleGameMode::APDBattleGameMode()
{
	GameStateType = EGameState::Ready;
	StageID = 1;
	RoundIndex = 0;

	CurPlayerSlotIndex = 0;
	CurEnemySlotIndex = 0;
	bIsPlayerTurn = false;
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

void APDBattleGameMode::DespawnUnit(const FGuid& InUnitGuid)
{
	if (ModelManager)
	{
		if (bIsPlayerTurn)
		{
			EnemyCharacters.Remove(CurEnemySlotIndex);
			++CurEnemySlotIndex;
		}
		else
		{
			PlayerCharacters.Remove(CurPlayerSlotIndex);
			++CurPlayerSlotIndex;
		}
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
	GetWorld()->GetTimerManager().SetTimer(TurnTimer, this, &APDBattleGameMode::ExecuteTurn, 5.0f, true, 0.0f);
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
	GetWorld()->GetTimerManager().ClearTimer(TurnTimer);
	UE_LOG(LogTemp, Warning, TEXT("[PD][BattleGameMode][EndGame] Game Clear!!"));
}

void APDBattleGameMode::StartStage(const int32 InStageID)
{
	StageID = InStageID;
	RoundIndex = 0;
	StartRound(RoundIndex);
}

void APDBattleGameMode::StartRound(const int32 InRoundIndex)
{
	RoundIndex = InRoundIndex;
	SpawnStageUnit();
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

	// ===== Ally Spawn =====
	if (RoundIndex == 0)	// Only stage start
	{
		TArray<int32> AllyUnitIdList;
		AllyUnitIdList.Add(1); // SlotIndex 0
		AllyUnitIdList.Add(2); // SlotIndex 1
		AllyUnitIdList.Add(3); // SlotIndex 2
		AllyUnitIdList.Add(4); // SlotIndex 3
		AllyUnitIdList.Add(5); // SlotIndex 4 - 0 대입시 Skip

		const TArray<APDCharacter*> SpawnedAllies =
			Spawner->SpawnAllyStageRoundUnits(StageID, RoundIndex, AllyUnitIdList);

		UE_LOG(LogTemp, Log, TEXT("[PD][BattleGameMode] StageRoundSpawner spawned %d ally units. (StageId:%d Round:%d)"),
			SpawnedAllies.Num(), StageID, RoundIndex);

		for (int32 i = 0; i < SpawnedAllies.Num(); ++i)
		{
			if (APDCharacter* Ch = SpawnedAllies[i])
			{
				if (AllyUnitIdList.Num() > i)
				{
					PlayerCharacters.Add(i, Ch);
				}

				const FVector Loc = Ch->GetActorLocation();
				const FRotator Rot = Ch->GetActorRotation();
				UE_LOG(LogTemp, Log,
					TEXT("[PD][BattleGameMode] Ally[%d] Spawned - Loc:%s Rot:%s"),
					i, *Loc.ToString(), *Rot.ToString());
			}
			else
			{
				UE_LOG(LogTemp, Warning,
					TEXT("[PD][BattleGameMode] Ally[%d] is null (spawn may have failed)."),
					i);
			}
		}
	}

	// ===== Enemy Spawn =====
	const TArray<FPDSpawnedStageUnitResult> SpawnedEnemies =
		Spawner->SpawnEnemyStageRoundUnits(StageID, RoundIndex);

	for (int32 i = 0; i < SpawnedEnemies.Num(); ++i)
	{
		int32 SlotIndex = SpawnedEnemies[i].SlotIndex;
		if (SlotIndex >= 0)
		{
			EnemyCharacters.Add(SlotIndex, SpawnedEnemies[i].Character);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("[PD][BattleGameMode] StageRoundSpawner spawned %d enemy units. (StageId:%d Round:%d)"),
		SpawnedEnemies.Num(), StageID, RoundIndex);
}

void APDBattleGameMode::ExecuteTurn()
{
	bIsPlayerTurn = !bIsPlayerTurn;
	if (bIsPlayerTurn)
	{
		ExecutePlayerTurn();
	}
	else
	{
		ExecuteEnemyTurn();
	}
}

void APDBattleGameMode::ExecutePlayerTurn()
{
	if (PlayerCharacters.Contains(CurPlayerSlotIndex) && EnemyCharacters.Contains(CurEnemySlotIndex))
	{
		APDCharacter* PlayerCharacter = PlayerCharacters[CurPlayerSlotIndex].Get();
		APDCharacter* EnemyCharacter = EnemyCharacters[CurEnemySlotIndex].Get();
		if (PlayerCharacter && EnemyCharacter)
		{
			PlayerCharacter->Attack(EnemyCharacter);
			return;
		}
	}

	EndGame();
}

void APDBattleGameMode::ExecuteEnemyTurn()
{
	if (PlayerCharacters.Contains(CurPlayerSlotIndex) && EnemyCharacters.Contains(CurEnemySlotIndex))
	{
		APDCharacter* PlayerCharacter = PlayerCharacters[CurPlayerSlotIndex].Get();
		APDCharacter* EnemyCharacter = EnemyCharacters[CurEnemySlotIndex].Get();
		if (PlayerCharacter && EnemyCharacter)
		{
			EnemyCharacter->Attack(PlayerCharacter);
			return;
		}
	}

	EndGame();
}

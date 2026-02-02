// Fill out your copyright notice in the Description page of Project Settings.


#include "PDBattleGameMode.h"

#include "../Manager/ModelManager.h"
#include "../Manager/PDStageRoundSpawner.h"
#include "../Character/PDCharacter.h"


APDBattleGameMode::APDBattleGameMode()
{
	GameStateType = EGameState::Ready;
	StageID = 1;
	RoundIndex = 0;
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

	GameStateType = EGameState::Ready;
	StartStage(StageID);
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

	UE_LOG(LogTemp, Log, TEXT("[PD][BattleGameMode] StageRoundSpawner spawned %d enemy units. (StageId:%d Round:%d)"),
		SpawnedEnemies.Num(), StageID, RoundIndex);
}

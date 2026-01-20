// Fill out your copyright notice in the Description page of Project Settings.


#include "PDBattleGameMode.h"

#include "../Manager/ModelManager.h"
#include "../Manager/PDStageRoundSpawner.h"
#include "../Character/PDCharacter.h"


void APDBattleGameMode::BeginPlay()
{
	Super::BeginPlay();

	//FVector Pos(4979.f, 5132.f, -193.f);
	//FRotator Rot(0.0f, 20.0f, 0.0f);
	//if (ModelManager)
	//{
	//	ModelManager->SpawnCharacter(1, Pos, Rot);
	//}
	SpawnStageUnit();
}

void APDBattleGameMode::SpawnStageUnit()
{
	if (!ModelManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PD][BattleGameMode][SpawnStageUnit] ModelManager is null."));
		return;
	}

	// Test: StageId / RoundIndex 로 라운드 스폰 테스트 (적군)
	// RoundIndex: 0-based (0->MonsterGroup1, 1->MonsterGroup2, 2->MonsterGroup3)
	const int32 TestStageId = 1;
	const int32 TestRoundIndex = 0;

	UPDStageRoundSpawner* Spawner = NewObject<UPDStageRoundSpawner>(this);
	Spawner->Initialize(ModelManager);

	const TArray<FPDSpawnedStageUnitResult> Spawned = Spawner->SpawnEnemyStageRoundUnits(TestStageId, TestRoundIndex);
	UE_LOG(LogTemp, Log, TEXT("[PD][BattleGameMode] StageRoundSpawner spawned %d enemy units. (StageId:%d Round:%d)"),
		Spawned.Num(), TestStageId, TestRoundIndex);
}

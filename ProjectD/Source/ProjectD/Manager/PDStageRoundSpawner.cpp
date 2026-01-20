#include "PDStageRoundSpawner.h"

#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"

#include "../Character/PDCharacter.h"
#include "../DataAsset/Stage/PDStageDataAsset.h"
#include "../DataAsset/Stage/StageTypes.h"
#include "../Table/PDMonsterGroupRow.h"
#include "../Table/PDStageRow.h"
#include "../Table/PDTableManagerSubsystem.h"
#include "ModelManager.h"

void UPDStageRoundSpawner::Initialize(UModelManager* InModelManager)
{
	ModelManager = InModelManager;
}

UWorld* UPDStageRoundSpawner::GetWorld() const
{
	if (const UObject* Outer = GetOuter())
	{
		return Outer->GetWorld();
	}
	return nullptr;
}

int32 UPDStageRoundSpawner::GetMonsterGroupIdByRoundIndex(const FPDStageRow& StageRow, int32 RoundIndex)
{
	switch (RoundIndex)
	{
	case 0: return StageRow.MonsterGroup1;
	case 1: return StageRow.MonsterGroup2;
	case 2: return StageRow.MonsterGroup3;
	default: return 0;
	}
}

void UPDStageRoundSpawner::CollectUnitIdsFromMonsterGroup(const FPDMonsterGroupRow& GroupRow, TArray<TPair<int32, int32>>& OutUnitIdAndLv)
{
	OutUnitIdAndLv.Reset();

	const TPair<int32, int32> Candidates[] =
	{
		TPair<int32, int32>(GroupRow.Monster1, GroupRow.Monster1_LV),
		TPair<int32, int32>(GroupRow.Monster2, GroupRow.Monster2_LV),
		TPair<int32, int32>(GroupRow.Monster3, GroupRow.Monster3_LV),
		TPair<int32, int32>(GroupRow.Monster4, GroupRow.Monster4_LV),
		TPair<int32, int32>(GroupRow.Monster5, GroupRow.Monster5_LV),
	};

	for (const TPair<int32, int32>& Pair : Candidates)
	{
		if (Pair.Key > 0)
		{
			OutUnitIdAndLv.Add(Pair);
		}
	}
}

void UPDStageRoundSpawner::CollectEnemySpawnPoints(const UPDStageDataAsset& StageAsset, int32 RoundIndex, TArray<FStageUnitSpawnPoint>& OutPoints)
{
	OutPoints.Reset();

	if (!StageAsset.Rounds.IsValidIndex(RoundIndex))
	{
		return;
	}

	const FStageRoundSpawnData& Round = StageAsset.Rounds[RoundIndex];
	OutPoints = Round.EnemySpawnPoints;

	OutPoints.Sort([](const FStageUnitSpawnPoint& A, const FStageUnitSpawnPoint& B)
	{
		return A.SlotIndex < B.SlotIndex;
	});
}

TArray<APDCharacter*> UPDStageRoundSpawner::SpawnAllyStageRoundUnits(int32 StageId, int32 RoundIndex)
{
	// TODO: 아군 스폰은 추후 파티/덱 데이터 기반으로 구현
	// 현재는 빈 함수로 유지(작업예정)
	UE_LOG(LogTemp, Warning, TEXT("[PD][StageRoundSpawner] SpawnAllyStageRoundUnits is not implemented yet. (StageId:%d Round:%d)"), StageId, RoundIndex);
	return TArray<APDCharacter*>();
}

TArray<FPDSpawnedStageUnitResult> UPDStageRoundSpawner::SpawnEnemyStageRoundUnits(int32 StageId, int32 RoundIndex)
{
	TArray<FPDSpawnedStageUnitResult> Results;

	UGameInstance* GI = UGameplayStatics::GetGameInstance(this);
	if (!GI)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PD][StageRoundSpawner] GameInstance is null. (StageId:%d Round:%d)"), StageId, RoundIndex);
		return Results;
	}

	UPDTableManagerSubsystem* TableManager = GI->GetSubsystem<UPDTableManagerSubsystem>();
	if (!TableManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PD][StageRoundSpawner] TableManagerSubsystem is null. (StageId:%d Round:%d)"), StageId, RoundIndex);
		return Results;
	}

	const FPDStageRow* StageRow = TableManager->GetStage(StageId);
	if (!StageRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PD][StageRoundSpawner] Invalid StageRow. (StageId:%d)"), StageId);
		return Results;
	}

	UPDStageDataAsset* StageAsset = TableManager->GetStageDataAssetByName(StageRow->DataAssetName);
	if (!StageAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PD][StageRoundSpawner] Failed to load StageDataAsset. (StageId:%d Asset:%s)"), StageId, *StageRow->DataAssetName);
		return Results;
	}

	const int32 MonsterGroupId = GetMonsterGroupIdByRoundIndex(*StageRow, RoundIndex);
	if (MonsterGroupId <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PD][StageRoundSpawner] MonsterGroupId is invalid. (StageId:%d Round:%d)"), StageId, RoundIndex);
		return Results;
	}

	const FPDMonsterGroupRow* GroupRow = TableManager->GetMonsterGroup(MonsterGroupId);
	if (!GroupRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PD][StageRoundSpawner] MonsterGroup not found. (GroupId:%d StageId:%d Round:%d)"), MonsterGroupId, StageId, RoundIndex);
		return Results;
	}

	if (!ModelManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PD][StageRoundSpawner] ModelManager is null. Call Initialize(ModelManager) first."));
		return Results;
	}

	TArray<TPair<int32, int32>> UnitIdAndLv;
	CollectUnitIdsFromMonsterGroup(*GroupRow, UnitIdAndLv);

	TArray<FStageUnitSpawnPoint> SpawnPoints;
	CollectEnemySpawnPoints(*StageAsset, RoundIndex, SpawnPoints);

	const int32 Count = FMath::Min(UnitIdAndLv.Num(), SpawnPoints.Num());
	if (Count <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PD][StageRoundSpawner] Nothing to spawn. Units:%d Points:%d (StageId:%d Round:%d)"),
			UnitIdAndLv.Num(),
			SpawnPoints.Num(),
			StageId,
			RoundIndex);
		return Results;
	}

	Results.Reserve(Count);

	for (int32 i = 0; i < Count; ++i)
	{
		const int32 UnitId = UnitIdAndLv[i].Key;
		const int32 UnitLv = UnitIdAndLv[i].Value;
		const FStageUnitSpawnPoint& P = SpawnPoints[i];

		FPDSpawnedStageUnitResult Item;
		Item.UnitId = UnitId;
		Item.UnitLv = UnitLv;
		Item.SlotIndex = P.SlotIndex;
		Item.SpawnLocation = P.Location;
		Item.SpawnRotation = P.Rotation;
		Item.Character = ModelManager->SpawnCharacter(UnitId, P.Location, P.Rotation);
		Results.Add(Item);
	}

	return Results;
}


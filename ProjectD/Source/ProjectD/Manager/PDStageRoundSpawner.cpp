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

namespace
{
	using FMonsterIntPtr = int32 FPDMonsterGroupRow::*;

	// MonsterGroup 슬롯 정의(필드 포인터). 슬롯이 늘어나면 이 배열에만 추가.
	static constexpr FMonsterIntPtr SlotUnitIdFields[] =
	{
		&FPDMonsterGroupRow::Slot0_UnitId,
		&FPDMonsterGroupRow::Slot1_UnitId,
		&FPDMonsterGroupRow::Slot2_UnitId,
		&FPDMonsterGroupRow::Slot3_UnitId,
		&FPDMonsterGroupRow::Slot4_UnitId,
	};

	static constexpr FMonsterIntPtr SlotUnitLvFields[] =
	{
		&FPDMonsterGroupRow::Slot0_UnitLv,
		&FPDMonsterGroupRow::Slot1_UnitLv,
		&FPDMonsterGroupRow::Slot2_UnitLv,
		&FPDMonsterGroupRow::Slot3_UnitLv,
		&FPDMonsterGroupRow::Slot4_UnitLv,
	};

	static_assert(UE_ARRAY_COUNT(SlotUnitIdFields) == UE_ARRAY_COUNT(SlotUnitLvFields), "SlotUnitIdFields and SlotUnitLvFields must have same size.");

	// MonsterGroup 슬롯 고정 개수 (현재 5). SlotUnit*Fields 정의와 항상 일치해야 한다.
	static constexpr int32 MonsterSlotCount = UE_ARRAY_COUNT(SlotUnitIdFields);
}

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
	OutUnitIdAndLv.SetNum(MonsterSlotCount);

	for (int32 i = 0; i < OutUnitIdAndLv.Num(); ++i)
	{
		const int32 UnitId = GroupRow.*SlotUnitIdFields[i];
		const int32 UnitLv = GroupRow.*SlotUnitLvFields[i];
		OutUnitIdAndLv[i] = TPair<int32, int32>(UnitId, UnitLv);
	}
}

void UPDStageRoundSpawner::CollectEnemySpawnPoints(const UPDStageDataAsset& StageAsset, int32 RoundIndex, TArray<FStageUnitSpawnPoint>& OutPoints, TArray<bool>& OutHasPoints)
{
	// MonsterSlotCount 기준. 중간이 비어도 인덱스가 당겨지지 않게 고정 길이로 유지한다.
	if (MonsterSlotCount <= 0)
	{
		OutPoints.Reset();
		OutHasPoints.Reset();
		return;
	}

	OutPoints.SetNum(MonsterSlotCount);
	OutHasPoints.Init(false, MonsterSlotCount);

	for (int32 i = 0; i < MonsterSlotCount; ++i)
	{
		OutPoints[i].SlotIndex = i;
		OutPoints[i].Location = FVector::ZeroVector;
		OutPoints[i].Rotation = FRotator::ZeroRotator;
	}

	if (!StageAsset.Rounds.IsValidIndex(RoundIndex))
	{
		return;
	}

	const FStageRoundSpawnData& Round = StageAsset.Rounds[RoundIndex];
	for (const FStageUnitSpawnPoint& P : Round.EnemySpawnPoints)
	{
		const int32 Slot = P.SlotIndex;
		if (Slot < 0 || Slot >= MonsterSlotCount)
		{
			UE_LOG(LogTemp, Warning, TEXT("[PD][StageRoundSpawner] Ignore enemy spawn point - invalid SlotIndex:%d (valid 0~%d). Loc:%s Rot:%s"),
				Slot,
				MonsterSlotCount - 1,
				*P.Location.ToString(),
				*P.Rotation.ToString());
			continue;
		}

		if (OutHasPoints[Slot])
		{
			UE_LOG(LogTemp, Warning, TEXT("[PD][StageRoundSpawner] Duplicate enemy spawn point SlotIndex:%d. Keeping first, ignoring this one. Loc:%s Rot:%s"),
				Slot,
				*P.Location.ToString(),
				*P.Rotation.ToString());
			continue;
		}

		OutPoints[Slot] = P;
		OutHasPoints[Slot] = true;
	}
}

void UPDStageRoundSpawner::CollectAllySpawnPoints(const UPDStageDataAsset& StageAsset, int32 RoundIndex, TArray<FStageUnitSpawnPoint>& OutPoints, TArray<bool>& OutHasPoints)
{
	// MonsterSlotCount 기준. 중간이 비어도 인덱스가 당겨지지 않게 고정 길이로 유지한다.
	if (MonsterSlotCount <= 0)
	{
		OutPoints.Reset();
		OutHasPoints.Reset();
		return;
	}

	OutPoints.SetNum(MonsterSlotCount);
	OutHasPoints.Init(false, MonsterSlotCount);

	for (int32 i = 0; i < MonsterSlotCount; ++i)
	{
		OutPoints[i].SlotIndex = i;
		OutPoints[i].Location = FVector::ZeroVector;
		OutPoints[i].Rotation = FRotator::ZeroRotator;
	}

	if (!StageAsset.Rounds.IsValidIndex(RoundIndex))
	{
		return;
	}

	const FStageRoundSpawnData& Round = StageAsset.Rounds[RoundIndex];
	for (const FStageUnitSpawnPoint& P : Round.AllySpawnPoints)
	{
		const int32 Slot = P.SlotIndex;
		if (Slot < 0 || Slot >= MonsterSlotCount)
		{
			UE_LOG(LogTemp, Warning, TEXT("[PD][StageRoundSpawner] Ignore ally spawn point - invalid SlotIndex:%d (valid 0~%d). Loc:%s Rot:%s"),
				Slot,
				MonsterSlotCount - 1,
				*P.Location.ToString(),
				*P.Rotation.ToString());
			continue;
		}

		if (OutHasPoints[Slot])
		{
			UE_LOG(LogTemp, Warning, TEXT("[PD][StageRoundSpawner] Duplicate ally spawn point SlotIndex:%d. Keeping first, ignoring this one. Loc:%s Rot:%s"),
				Slot,
				*P.Location.ToString(),
				*P.Rotation.ToString());
			continue;
		}

		OutPoints[Slot] = P;
		OutHasPoints[Slot] = true;
	}
}

TArray<APDCharacter*> UPDStageRoundSpawner::SpawnAllyStageRoundUnits(int32 StageId, int32 RoundIndex, const TArray<int32>& UnitIdList)
{
	TArray<APDCharacter*> Results;

	if (UnitIdList.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PD][StageRoundSpawner] SpawnAllyStageRoundUnits called with empty UnitIdList. (StageId:%d Round:%d)"), StageId, RoundIndex);
		return Results;
	}

	UGameInstance* GI = UGameplayStatics::GetGameInstance(this);
	if (!GI)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PD][StageRoundSpawner] GameInstance is null (ally). (StageId:%d Round:%d)"), StageId, RoundIndex);
		return Results;
	}

	UPDTableManagerSubsystem* TableManager = GI->GetSubsystem<UPDTableManagerSubsystem>();
	if (!TableManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PD][StageRoundSpawner] TableManagerSubsystem is null (ally). (StageId:%d Round:%d)"), StageId, RoundIndex);
		return Results;
	}

	const FPDStageRow* StageRow = TableManager->GetStage(StageId);
	if (!StageRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PD][StageRoundSpawner] Invalid StageRow (ally). (StageId:%d)"), StageId);
		return Results;
	}

	UPDStageDataAsset* StageAsset = TableManager->GetStageDataAssetByName(StageRow->DataAssetName);
	if (!StageAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PD][StageRoundSpawner] Failed to load StageDataAsset (ally). (StageId:%d Asset:%s)"), StageId, *StageRow->DataAssetName);
		return Results;
	}

	if (!ModelManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PD][StageRoundSpawner] ModelManager is null (ally). Call Initialize(ModelManager) first."));
		return Results;
	}

	TArray<FStageUnitSpawnPoint> SpawnPoints;
	TArray<bool> HasSpawnPoints;
	CollectAllySpawnPoints(*StageAsset, RoundIndex, SpawnPoints, HasSpawnPoints);

	// UnitIdList 인덱스 == SlotIndex 매칭. MonsterSlotCount를 넘는 유닛은 무시.
	const int32 MaxIndex = FMath::Min(UnitIdList.Num(), MonsterSlotCount);

	for (int32 i = 0; i < MaxIndex; ++i)
	{
		const int32 UnitId = UnitIdList[i];
		if (UnitId <= 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("[PD][StageRoundSpawner] Skip ally spawn - invalid UnitId <= 0. (StageId:%d Round:%d Slot:%d UnitId:%d)"),
				StageId, RoundIndex, i, UnitId);
			continue;
		}

		if (!HasSpawnPoints.IsValidIndex(i) || !HasSpawnPoints[i])
		{
			UE_LOG(LogTemp, Warning, TEXT("[PD][StageRoundSpawner] Cannot spawn ally - missing spawn point. (StageId:%d Round:%d Slot:%d UnitId:%d)"),
				StageId, RoundIndex, i, UnitId);
			continue;
		}

		if (!TableManager->GetUnit(UnitId))
		{
			UE_LOG(LogTemp, Warning, TEXT("[PD][StageRoundSpawner] Cannot spawn ally - UnitId is empty (no FPDUnitRow in UnitTable). (StageId:%d Round:%d Slot:%d UnitId:%d)"),
				StageId, RoundIndex, i, UnitId);
			continue;
		}

		const FStageUnitSpawnPoint& P = SpawnPoints[i];
		APDCharacter* Character = ModelManager->SpawnCharacter(UnitId, P.Location, P.Rotation);
		if (!Character)
		{
			UE_LOG(LogTemp, Warning, TEXT("[PD][StageRoundSpawner] Cannot spawn ally - SpawnCharacter failed. (StageId:%d Round:%d Slot:%d UnitId:%d Loc:%s Rot:%s)"),
				StageId, RoundIndex, i, UnitId, *P.Location.ToString(), *P.Rotation.ToString());
			continue;
		}

		Results.Add(Character);
	}

	if (UnitIdList.Num() > MonsterSlotCount)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PD][StageRoundSpawner] SpawnAllyStageRoundUnits: UnitIdList has more entries than supported slots. Extra entries are ignored. (StageId:%d Round:%d UnitCount:%d MaxSlots:%d)"),
			StageId, RoundIndex, UnitIdList.Num(), MonsterSlotCount);
	}

	return Results;
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
	TArray<bool> HasSpawnPoints;
	CollectEnemySpawnPoints(*StageAsset, RoundIndex, SpawnPoints, HasSpawnPoints);

	// 인덱스 보존을 위해 MonsterGroup 슬롯 개수(SlotCount)만큼 결과를 만든다.
	Results.Reserve(UnitIdAndLv.Num());

	for (int32 i = 0; i < UnitIdAndLv.Num(); ++i)
	{
		const int32 UnitId = UnitIdAndLv[i].Key;
		const int32 UnitLv = UnitIdAndLv[i].Value;

		FPDSpawnedStageUnitResult Item;
		Item.UnitId = UnitId;
		Item.UnitLv = UnitLv;
		Item.SlotIndex = i; // 슬롯(0~4) 고정 유지

		if (!HasSpawnPoints.IsValidIndex(i) || !HasSpawnPoints[i])
		{
			UE_LOG(LogTemp, Warning, TEXT("[PD][StageRoundSpawner] Cannot spawn enemy - missing spawn point. (StageId:%d Round:%d MonsterIndex:%d Slot:%d)"),
				StageId, RoundIndex, i + 1, i);
			Results.Add(Item);
			continue;
		}

		const FStageUnitSpawnPoint& P = SpawnPoints[i];
		Item.SpawnLocation = P.Location;
		Item.SpawnRotation = P.Rotation;

		if (UnitLv < 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("[PD][StageRoundSpawner] Cannot spawn enemy - UnitLv < 0. (StageId:%d Round:%d MonsterIndex:%d Slot:%d UnitId:%d UnitLv:%d)"),
				StageId, RoundIndex, i + 1, i, UnitId, UnitLv);
			Results.Add(Item);
			continue;
		}

		if (!TableManager->GetUnit(UnitId))
		{
			UE_LOG(LogTemp, Warning, TEXT("[PD][StageRoundSpawner] Cannot spawn enemy - UnitId is empty (no FPDUnitRow in UnitTable). (StageId:%d Round:%d MonsterIndex:%d Slot:%d UnitId:%d UnitLv:%d)"),
				StageId, RoundIndex, i + 1, i, UnitId, UnitLv);
			Results.Add(Item);
			continue;
		}

		Item.Character = ModelManager->SpawnCharacter(UnitId, P.Location, P.Rotation);
		if (!Item.Character)
		{
			UE_LOG(LogTemp, Warning, TEXT("[PD][StageRoundSpawner] Cannot spawn enemy - SpawnCharacter failed. (StageId:%d Round:%d MonsterIndex:%d Slot:%d UnitId:%d UnitLv:%d)"),
				StageId, RoundIndex, i + 1, i, UnitId, UnitLv);
		}

		Results.Add(Item);
	}

	return Results;
}


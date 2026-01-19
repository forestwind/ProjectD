// Fill out your copyright notice in the Description page of Project Settings.

#include "PDTableManagerSubsystem.h"
#include "Engine/DataTable.h"
#include "Misc/PackageName.h"

namespace
{
	// "DA_Unit_001" 처럼 "에셋 이름"만 허용
	static FString MakeUnitDataAssetObjectPathFromName(FString In)
	{
		In.TrimStartAndEndInline();
		if (In.IsEmpty())
		{
			return FString();
		}
		
		const FString AssetName = FPackageName::GetShortName(In);
		if (AssetName.IsEmpty())
		{
			return FString();
		}

		return FString::Printf(TEXT("/Game/DataAsset/Unit/%s.%s"), *AssetName, *AssetName);
	}
}

void UPDTableManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	UnitDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Table/DataTable/DT_Unit"));
	UnitStatDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Table/DataTable/DT_UnitStat"));
	UnitLevelDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Table/DataTable/DT_UnitLevel"));
	StageDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Table/DataTable/DT_Stage"));
	MonsterGroupDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Table/DataTable/DT_MonsterGroup"));
	
	// DataTable을 맵으로 변환 (RowName 의존 제거)
	BuildUnitMap();
	BuildUnitStatMap();
	BuildUnitLevelMap();
	BuildStageMap();
	BuildMonsterGroupMap();
}

void UPDTableManagerSubsystem::Deinitialize()
{
	UnitDataAssetCache.Empty();
	UnitMap.Empty();
	UnitStatMap.Empty();
	UnitLevelMap.Empty();
	StageMap.Empty();
	MonsterGroupMap.Empty();

	UnitDataTable = nullptr;
	UnitStatDataTable = nullptr;
	UnitLevelDataTable = nullptr;
	StageDataTable = nullptr;
	MonsterGroupDataTable = nullptr;
	Super::Deinitialize();
}

void UPDTableManagerSubsystem::BuildUnitMap()
{
	UnitMap.Empty();

	if (!UnitDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PD][TableManager] UnitDataTable is null!"));
		return;
	}

	int32 LoadedCount = 0;
	TArray<FPDUnitRow*> AllRows;
	UnitDataTable->GetAllRows(TEXT("UPDTableManagerSubsystem::BuildUnitMap"), AllRows);

	for (const FPDUnitRow* Row : AllRows)
	{
		if (!Row) continue;
		if (Row->ID <= 0) continue;
		if (UnitMap.Contains(Row->ID)) continue;

		UnitMap.Add(Row->ID, Row);
		++LoadedCount;
	}

	UE_LOG(LogTemp, Log, TEXT("[PD][TableManager] UnitMap built. Total entries: %d"), LoadedCount);
}

void UPDTableManagerSubsystem::BuildUnitStatMap()
{
	UnitStatMap.Empty();

	if (!UnitStatDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PD][TableManager] UnitStatDataTable is null!"));
		return;
	}

	int32 LoadedCount = 0;
	TArray<FPDUnitStatRow*> AllRows;
	UnitStatDataTable->GetAllRows(TEXT("UPDTableManagerSubsystem::BuildUnitStatMap"), AllRows);

	for (const FPDUnitStatRow* Row : AllRows)
	{
		if (!Row) continue;
		if (Row->ID <= 0) continue;
		if (UnitStatMap.Contains(Row->ID)) continue;

		UnitStatMap.Add(Row->ID, Row);
		++LoadedCount;
	}

	UE_LOG(LogTemp, Log, TEXT("[PD][TableManager] UnitStatMap built. Total entries: %d"), LoadedCount);
}

void UPDTableManagerSubsystem::BuildUnitLevelMap()
{
	UnitLevelMap.Empty();

	if (!UnitLevelDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PD][TableManager] UnitLevelDataTable is null!"));
		return;
	}

	int32 LoadedCount = 0;
	TArray<FPDUnitLevelRow*> AllRows;
	UnitLevelDataTable->GetAllRows(TEXT("UPDTableManagerSubsystem::BuildUnitLevelMap"), AllRows);

	for (const FPDUnitLevelRow* Row : AllRows)
	{
		if (!Row) continue;
		if (Row->Level <= 0) continue;
		if (UnitLevelMap.Contains(Row->Level)) continue;

		UnitLevelMap.Add(Row->Level, Row);
		++LoadedCount;
	}

	UE_LOG(LogTemp, Log, TEXT("[PD][TableManager] UnitLevelMap built. Total entries: %d"), LoadedCount);
}

void UPDTableManagerSubsystem::BuildStageMap()
{
	StageMap.Empty();

	if (!StageDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PD][TableManager] StageDataTable is null!"));
		return;
	}

	int32 LoadedCount = 0;
	TArray<FPDStageRow*> AllRows;
	StageDataTable->GetAllRows(TEXT("UPDTableManagerSubsystem::BuildStageMap"), AllRows);

	for (const FPDStageRow* Row : AllRows)
	{
		if (!Row) continue;
		if (Row->ID <= 0) continue;
		if (StageMap.Contains(Row->ID)) continue;

		StageMap.Add(Row->ID, Row);
		++LoadedCount;
	}

	UE_LOG(LogTemp, Log, TEXT("[PD][TableManager] StageMap built. Total entries: %d"), LoadedCount);
}

void UPDTableManagerSubsystem::BuildMonsterGroupMap()
{
	MonsterGroupMap.Empty();

	if (!MonsterGroupDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PD][TableManager] MonsterGroupDataTable is null!"));
		return;
	}

	int32 LoadedCount = 0;
	TArray<FPDMonsterGroupRow*> AllRows;
	MonsterGroupDataTable->GetAllRows(TEXT("UPDTableManagerSubsystem::BuildMonsterGroupMap"), AllRows);

	for (const FPDMonsterGroupRow* Row : AllRows)
	{
		if (!Row) continue;
		if (Row->ID <= 0) continue;
		if (MonsterGroupMap.Contains(Row->ID)) continue;

		MonsterGroupMap.Add(Row->ID, Row);
		++LoadedCount;
	}

	UE_LOG(LogTemp, Log, TEXT("[PD][TableManager] MonsterGroupMap built. Total entries: %d"), LoadedCount);
}

const FPDUnitRow* UPDTableManagerSubsystem::GetUnit(int32 UnitID) const
{
	if (UnitID <= 0) return nullptr;
	const FPDUnitRow* const* FoundRow = UnitMap.Find(UnitID);
	return FoundRow ? *FoundRow : nullptr;
}

const FPDUnitStatRow* UPDTableManagerSubsystem::GetUnitStat(int32 StatID) const
{
	if (StatID <= 0) return nullptr;
	const FPDUnitStatRow* const* FoundRow = UnitStatMap.Find(StatID);
	return FoundRow ? *FoundRow : nullptr;
}

const FPDUnitLevelRow* UPDTableManagerSubsystem::GetUnitLevel(int32 Level) const
{
	if (Level <= 0) return nullptr;
	const FPDUnitLevelRow* const* FoundRow = UnitLevelMap.Find(Level);
	return FoundRow ? *FoundRow : nullptr;
}

const FPDStageRow* UPDTableManagerSubsystem::GetStage(int32 StageID) const
{
	if (StageID <= 0) return nullptr;
	const FPDStageRow* const* FoundRow = StageMap.Find(StageID);
	return FoundRow ? *FoundRow : nullptr;
}

const FPDMonsterGroupRow* UPDTableManagerSubsystem::GetMonsterGroup(int32 MonsterGroupID) const
{
	if (MonsterGroupID <= 0) return nullptr;
	const FPDMonsterGroupRow* const* FoundRow = MonsterGroupMap.Find(MonsterGroupID);
	return FoundRow ? *FoundRow : nullptr;
}

UPDUnitDataAsset* UPDTableManagerSubsystem::GetUnitDataAssetByName(const FString& AssetName, bool bForceReload /*=false*/)
{
	const FString ObjectPathStr = MakeUnitDataAssetObjectPathFromName(AssetName);
	if (ObjectPathStr.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("[PD][TableManager] GetUnitDataAssetByName only accepts asset name. Input: %s"), *AssetName);
		return nullptr;
	}

	// 캐시 키는 "에셋 이름" 기준으로 통일 (입력 공백/케이스 차이 방지)
	const FString AssetNameOnly = FPackageName::GetShortName(ObjectPathStr);
	const FName CacheKey(*AssetNameOnly);

	if (!bForceReload)
	{
		if (TObjectPtr<UPDUnitDataAsset>* Found = UnitDataAssetCache.Find(CacheKey))
		{
			if (IsValid(*Found))
			{
				return Found->Get();
			}

			// 캐시에 있는데 무효면 제거 후 재로드
			UnitDataAssetCache.Remove(CacheKey);
		}
	}
	else
	{
		UnitDataAssetCache.Remove(CacheKey);
	}

	UPDUnitDataAsset* Loaded = LoadObject<UPDUnitDataAsset>(nullptr, *ObjectPathStr);
	if (!Loaded)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PD][TableManager] Failed to load UnitDataAsset. Name: %s Path: %s"), *AssetNameOnly, *ObjectPathStr);
		return nullptr;
	}

	UnitDataAssetCache.Add(CacheKey, Loaded);
	return Loaded;
}

// Fill out your copyright notice in the Description page of Project Settings.

#include "PDTableManagerSubsystem.h"
#include "Engine/DataTable.h"

void UPDTableManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	// DT_UnitStats 자동 로드
	UnitStatsDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Unit/DataTable/DT_UnitStats"));
	
	// DataTable을 맵으로 변환 (RowName 의존 제거)
	BuildUnitStatsMap();
}

void UPDTableManagerSubsystem::Deinitialize()
{
	UnitStatsMap.Empty();
	UnitStatsDataTable = nullptr;
	Super::Deinitialize();
}

void UPDTableManagerSubsystem::BuildUnitStatsMap()
{
	UnitStatsMap.Empty();

	if (!UnitStatsDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PD][TableManager] UnitStatsDataTable is null!"));
		return;
	}

	int32 LoadedCount = 0;

	TArray<FPDUnitStatsRow*> AllRows;
	UnitStatsDataTable->GetAllRows(TEXT("UPDTableManagerSubsystem::BuildUnitStatsMap"), AllRows);

	// DataTable의 모든 Row를 순회하며 ID를 키로 맵에 저장
	for (const FPDUnitStatsRow* Row : AllRows)
	{
		if (!Row) continue;
		
		if (Row->ID <= 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("[PD][TableManager] Invalid Unit ID (<=0). Row skipped."));
			continue;
		}

		if (UnitStatsMap.Contains(Row->ID))
		{
			UE_LOG(LogTemp, Warning, TEXT("[PD][TableManager] Duplicate Unit ID detected: %d. Row skipped."), Row->ID);
			continue;
		}

		UnitStatsMap.Add(Row->ID, Row);
		++LoadedCount;
		UE_LOG(LogTemp, Log, TEXT("[PD][TableManager] Added Unit Stats - ID: %d, Attack: %d, Defense: %d"), Row->ID, Row->Attack, Row->Defense);
	}

	UE_LOG(LogTemp, Warning, TEXT("[PD][TableManager] UnitStatsMap built successfully! Total entries: %d"), LoadedCount);
}

const FPDUnitStatsRow* UPDTableManagerSubsystem::GetUnitStats(int32 UnitID) const
{
	if (UnitID <= 0)
	{
		return nullptr;
	}

	// 맵에서 직접 조회
	const FPDUnitStatsRow* const* FoundRow = UnitStatsMap.Find(UnitID);
	return FoundRow ? *FoundRow : nullptr;
}

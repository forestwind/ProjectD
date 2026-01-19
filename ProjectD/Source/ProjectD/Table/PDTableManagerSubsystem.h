// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
//#include "Engine/DataTable.h"
#include "PDUnitRow.h"
#include "PDUnitStatRow.h"
#include "PDUnitLevelRow.h"
#include "PDStageRow.h"
#include "PDMonsterGroupRow.h"
#include "../DataAsset/PDUnitDataAsset.h"
#include "PDTableManagerSubsystem.generated.h"

class UDataTable;
/**
 * 
 */
UCLASS()
class PROJECTD_API UPDTableManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

protected:

	// DataTable
	// 데이터 테이블은 초기화 시 자동 로드

	// UnitDataTable
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Table|Unit")
	TObjectPtr<UDataTable> UnitDataTable;

	// UnitStatDataTable
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Table|Unit")
	TObjectPtr<UDataTable> UnitStatDataTable;

	// UnitLevelDataTable
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Table|Unit")
	TObjectPtr<UDataTable> UnitLevelDataTable;

	// StageDataTable
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Table|Stage")
	TObjectPtr<UDataTable> StageDataTable;

	// MonsterGroupDataTable
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Table|Stage")
	TObjectPtr<UDataTable> MonsterGroupDataTable;

	// ID 기반 빠른 조회를 위한 캐시 맵
	TMap<int32, const FPDUnitRow*> UnitMap;
	TMap<int32, const FPDUnitStatRow*> UnitStatMap;
	TMap<int32, const FPDUnitLevelRow*> UnitLevelMap; // Key: Level
	TMap<int32, const FPDStageRow*> StageMap;
	TMap<int32, const FPDMonsterGroupRow*> MonsterGroupMap;

	// DataTable을 맵으로 변환
	void BuildUnitMap();
	void BuildUnitStatMap();
	void BuildUnitLevelMap();
	void BuildStageMap();
	void BuildMonsterGroupMap();

	// DataAsset
	// 로드된 Unit DataAsset 캐시 (GC 방지용)
	UPROPERTY(Transient)
	TMap<FName, TObjectPtr<UPDUnitDataAsset>> UnitDataAssetCache;

public:
	
	// Unit ID로 유닛 데이터 조회
	const FPDUnitRow* GetUnit(int32 UnitID) const;

	// Stat ID로 기본 스탯 데이터 조회
	const FPDUnitStatRow* GetUnitStat(int32 StatID) const;

	// Level로 레벨 데이터 조회
	const FPDUnitLevelRow* GetUnitLevel(int32 Level) const;

	// Stage ID로 스테이지 데이터 조회
	const FPDStageRow* GetStage(int32 StageID) const;

	// MonsterGroup ID로 몬스터 그룹 데이터 조회
	const FPDMonsterGroupRow* GetMonsterGroup(int32 MonsterGroupID) const;

	UFUNCTION(BlueprintCallable, Category = "Table|Unit")
	UDataTable* GetUnitDataTable() const { return UnitDataTable; }

	UFUNCTION(BlueprintCallable, Category = "Table|Unit")
	UDataTable* GetUnitStatDataTable() const { return UnitStatDataTable; }

	UFUNCTION(BlueprintCallable, Category = "Table|Unit")
	UDataTable* GetUnitLevelDataTable() const { return UnitLevelDataTable; }

	UFUNCTION(BlueprintCallable, Category = "Table|Stage")
	UDataTable* GetStageDataTable() const { return StageDataTable; }

	UFUNCTION(BlueprintCallable, Category = "Table|Stage")
	UDataTable* GetMonsterGroupDataTable() const { return MonsterGroupDataTable; }


	// DataAsset 이름으로 유닛 DataAsset 로드 및 캐시 반환
	UFUNCTION(BlueprintCallable, Category = "Table|DataAsset")
	UPDUnitDataAsset* GetUnitDataAssetByName(const FString& AssetName, bool bForceReload = false);
};

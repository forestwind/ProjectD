// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Engine/DataTable.h"
#include "PDUnitRow.h"
#include "PDUnitStatRow.h"
#include "PDUnitLevelRow.h"
#include "PDStageRow.h"
#include "PDMonsterGroupRow.h"
#include "PDTableManagerSubsystem.generated.h"

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
	// 유닛 DataTable 참조 (초기화 시 자동 로드)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Table|Unit")
	TObjectPtr<UDataTable> UnitDataTable;

	// 유닛 기본스탯 DataTable 참조 (초기화 시 자동 로드)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Table|Unit")
	TObjectPtr<UDataTable> UnitStatDataTable;

	// 유닛 레벨 DataTable 참조 (초기화 시 자동 로드)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Table|Unit")
	TObjectPtr<UDataTable> UnitLevelDataTable;

	// 스테이지 DataTable 참조 (초기화 시 자동 로드)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Table|Stage")
	TObjectPtr<UDataTable> StageDataTable;

	// 몬스터 그룹 DataTable 참조 (초기화 시 자동 로드)
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
};

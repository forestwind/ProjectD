// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
//#include "Engine/DataTable.h"
#include "PDUnitRow.h"
#include "PDUnitStatRow.h"
#include "PDUnitLevelRow.h"
#include "PDStageRow.h"
#include "PDBattleItemRow.h"
#include "PDUIBaseRow.h"
#include "DataAsset/PDUnitDataAsset.h"
#include "DataAsset/Stage/PDStageDataAsset.h"
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

	// DataTable — Initialize() 시 자동 로드
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Table|Unit")
	TObjectPtr<UDataTable> UnitDataTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Table|Unit")
	TObjectPtr<UDataTable> UnitStatDataTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Table|Unit")
	TObjectPtr<UDataTable> UnitLevelDataTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Table|Stage")
	TObjectPtr<UDataTable> StageDataTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Table|Item")
	TObjectPtr<UDataTable> BattleItemDataTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Table|UI")
	TObjectPtr<UDataTable> UIBaseDataTable;

	// ID 기반 빠른 조회를 위한 캐시 맵 (Key: 각 Row의 ID)
	TMap<int32, const FPDUnitRow*> UnitMap;
	TMap<int32, const FPDUnitStatRow*> UnitStatMap;
	TMap<int32, const FPDUnitLevelRow*> UnitLevelMap;
	TMap<int32, const FPDStageRow*> StageMap;
	TMap<int32, const FPDBattleItemRow*> BattleItemMap;
	TMap<EUIType, const FPDUIBase*> UIBaseMap;

	// DataTable을 맵으로 변환
	void BuildUnitMap();
	void BuildUnitStatMap();
	void BuildUnitLevelMap();
	void BuildStageMap();
	void BuildBattleItemMap();
	void BuildUIBaseMap();

	// DataAsset
	// 로드된 Unit DataAsset 캐시 (GC 방지용)
	UPROPERTY(Transient)
	TMap<FName, TObjectPtr<UPDUnitDataAsset>> UnitDataAssetCache;

	UPROPERTY(Transient)
	TMap<FName, TObjectPtr<UPDStageDataAsset>> StageDataAssetCache;

public:
	
	// Unit ID로 유닛 데이터 조회
	const FPDUnitRow* GetUnit(int32 UnitID) const;

	// Stat ID로 기본 스탯 데이터 조회
	const FPDUnitStatRow* GetUnitStat(int32 StatID) const;

	// Level로 레벨 데이터 조회
	const FPDUnitLevelRow* GetUnitLevel(int32 Level) const;

	// Stage ID로 스테이지 데이터 조회
	const FPDStageRow* GetStage(int32 StageID) const;

	// Item ID로 배틀 아이템 데이터 조회
	const FPDBattleItemRow* GetBattleItem(int32 ItemID) const;

	// UIType으로 UI 데이터 조회
	const FPDUIBase* GetUIBase(EUIType UIType) const;

	// UIType에 해당하는 위젯 블루프린트 클래스 로드
	UClass* GetWidgetClass(EUIType UIType) const;

	// 유닛 BP 조회 — 첫 호출 시 DataAsset을 캐시에 로드
	UClass* GetUnitBP(int32 InUnitID);

	UFUNCTION(BlueprintCallable, Category = "Table|Unit")
	UDataTable* GetUnitDataTable() const { return UnitDataTable; }

	UFUNCTION(BlueprintCallable, Category = "Table|Unit")
	UDataTable* GetUnitStatDataTable() const { return UnitStatDataTable; }

	UFUNCTION(BlueprintCallable, Category = "Table|Unit")
	UDataTable* GetUnitLevelDataTable() const { return UnitLevelDataTable; }

	UFUNCTION(BlueprintCallable, Category = "Table|Stage")
	UDataTable* GetStageDataTable() const { return StageDataTable; }

	UFUNCTION(BlueprintCallable, Category = "Table|Item")
	UDataTable* GetBattleItemDataTable() const { return BattleItemDataTable; }

	UFUNCTION(BlueprintCallable, Category = "Table|UI")
	UDataTable* GetUIBaseDataTable() const { return UIBaseDataTable; }

	// DataAsset 이름으로 유닛 DataAsset 로드 및 캐시 반환
	UFUNCTION(BlueprintCallable, Category = "Table|DataAsset")
	UPDUnitDataAsset* GetUnitDataAssetByName(const FString& AssetName, bool bForceReload = false);

	UFUNCTION(BlueprintCallable, Category = "Table|DataAsset")
	UPDStageDataAsset* GetStageDataAssetByName(const FString& AssetName, bool bForceReload = false);
};

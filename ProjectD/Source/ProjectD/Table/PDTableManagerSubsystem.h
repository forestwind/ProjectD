// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Engine/DataTable.h"
#include "../Unit/PDUnitStatsRow.h"
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
	// 유닛 스탯 DataTable 참조 (초기화 시 자동 로드)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Table|Unit")
	TObjectPtr<UDataTable> UnitStatsDataTable;

	// ID 기반 빠른 조회를 위한 캐시 맵
	TMap<int32, const FPDUnitStatsRow*> UnitStatsMap;

	// DataTable을 맵으로 변환
	void BuildUnitStatsMap();

	// TODO: 나중에 추가할 다른 테이블들도 여기에 추가
	// 예: ItemStatsDataTable, SkillStatsDataTable 등

public:
	
	// 유닛 ID로 스탯 데이터 조회
	const FPDUnitStatsRow* GetUnitStats(int32 UnitID) const;

	UFUNCTION(BlueprintCallable, Category = "Table|Unit")
	UDataTable* GetUnitStatsDataTable() const { return UnitStatsDataTable; }
};

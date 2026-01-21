// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PDMonsterGroupRow.generated.h"

// CSV(DataTable)로 관리하는 몬스터 그룹 데이터 (Content/Unit/Raw/CSV/MonsterGroup.csv)
// NOTE: CSV(DataTable) 매핑을 위해 슬롯을 펼친 형태로 유지

USTRUCT(BlueprintType)
struct PROJECTD_API FPDMonsterGroupRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MonsterGroup")
	int32 ID = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MonsterGroup")
	int32 Slot0_UnitId = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MonsterGroup")
	int32 Slot0_UnitLv = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MonsterGroup")
	int32 Slot1_UnitId = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MonsterGroup")
	int32 Slot1_UnitLv = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MonsterGroup")
	int32 Slot2_UnitId = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MonsterGroup")
	int32 Slot2_UnitLv = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MonsterGroup")
	int32 Slot3_UnitId = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MonsterGroup")
	int32 Slot3_UnitLv = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MonsterGroup")
	int32 Slot4_UnitId = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MonsterGroup")
	int32 Slot4_UnitLv = 0;
};


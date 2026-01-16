// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PDUnitStatRow.generated.h"

// CSV(DataTable)로 관리하는 유닛 기본 스텟 데이터 (Content/Unit/Raw/CSV/UnitStat.csv)
USTRUCT(BlueprintType)
struct PROJECTD_API FPDUnitStatRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Stats")
	int32 ID = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Stats")
	int32 HP = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Stats")
	int32 Attack = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Stats")
	int32 Defense = 0;
};


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PDUnitStatsRow.generated.h"

// CSV(DataTable)로 관리하는 유닛 스텟 데이터
USTRUCT(BlueprintType)
struct PROJECTD_API FPDUnitStatsRow : public FTableRowBase
{
	GENERATED_BODY()

	// Unit Stat ID (숫자)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Stats")
	int32 ID = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Stats")
	int32 Attack = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Stats")
	int32 Defense = 0;
};


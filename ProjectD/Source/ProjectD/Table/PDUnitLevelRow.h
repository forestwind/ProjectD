// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PDUnitLevelRow.generated.h"

// CSV(DataTable)로 관리하는 유닛 레벨 데이터 (Content/Unit/Raw/CSV/UnitLevel.csv)
USTRUCT(BlueprintType)
struct PROJECTD_API FPDUnitLevelRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Level")
	int32 Level = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Level")
	int32 Exp = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Level")
	int32 TotalExp = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Level")
	bool IsMaxLevel = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Level")
	int32 AddHP = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Level")
	int32 AddAttack = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Level")
	int32 AddDefence = 0;
};


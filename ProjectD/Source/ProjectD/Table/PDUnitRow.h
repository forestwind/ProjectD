// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PDUnitRow.generated.h"

// CSV(DataTable)로 관리하는 유닛 데이터 (Content/Unit/Raw/CSV/Unit.csv)
// - 첫 컬럼 Name(RowName)은 DataTable의 RowName으로 사용됨
USTRUCT(BlueprintType)
struct PROJECTD_API FPDUnitRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Unit")
	int32 ID = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Unit")
	FString UnitName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Unit")
	int32 StatID = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Unit")
	int32 KillExp = 0;
};


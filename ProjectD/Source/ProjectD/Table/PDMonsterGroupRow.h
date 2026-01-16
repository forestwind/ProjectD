// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PDMonsterGroupRow.generated.h"

// CSV(DataTable)로 관리하는 몬스터 그룹 데이터 (Content/Unit/Raw/CSV/MonsterGroup.csv)
// 주의: CSV 헤더에 "Monter1_LV" 오타가 있어 동일한 이름으로 맞춤
USTRUCT(BlueprintType)
struct PROJECTD_API FPDMonsterGroupRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MonsterGroup")
	int32 ID = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MonsterGroup")
	int32 Monster1 = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MonsterGroup")
	int32 Monster1_LV = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MonsterGroup")
	int32 Monster2 = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MonsterGroup")
	int32 Monster2_LV = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MonsterGroup")
	int32 Monster3 = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MonsterGroup")
	int32 Monster3_LV = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MonsterGroup")
	int32 Monster4 = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MonsterGroup")
	int32 Monster4_LV = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MonsterGroup")
	int32 Monster5 = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MonsterGroup")
	int32 Monster5_LV = 0;
};


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PDStageRow.generated.h"

// CSV(DataTable)로 관리하는 스테이지 데이터 (Content/Unit/Raw/CSV/Stage.csv)
USTRUCT(BlueprintType)
struct PROJECTD_API FPDStageRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Stage")
	int32 ID = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Stage")
	FString StageName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Stage")
	int32 MonsterGroup1 = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Stage")
	int32 MonsterGroup2 = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Stage")
	int32 MonsterGroup3 = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stage")
	FString DataAssetName;
};


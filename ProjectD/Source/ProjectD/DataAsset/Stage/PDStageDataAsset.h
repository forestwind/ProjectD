// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "StageTypes.h"
#include "PDStageDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API UPDStageDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	// 라운드별 스폰 데이터
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stage")
	TArray<FStageRoundSpawnData> Rounds;
};
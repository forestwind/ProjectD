// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StageTypes.generated.h"

USTRUCT(BlueprintType)
struct FStageUnitSpawnPoint
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	FVector Location = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	FRotator Rotation = FRotator::ZeroRotator;

};

USTRUCT(BlueprintType)
struct FStageSpawnData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	TArray<FStageUnitSpawnPoint> EnemySpawnPoints;
};
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../PDGameModeBase.h"
#include "PDBattleGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API APDBattleGameMode : public APDGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	void SpawnStageUnit();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../PDGameModeBase.h"
#include "PDBattleGameMode.generated.h"

UENUM()
enum class EGameState : uint8
{
	Ready,
	Play,
	End,
	Max
};

/**
 * 
 */
UCLASS()
class PROJECTD_API APDBattleGameMode : public APDGameModeBase
{
	GENERATED_BODY()
	
public:
	APDBattleGameMode();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void BeginPlay() override;

	void StartStage(const int32 InStageID);
	void StartRound(const int32 InRoundIndex);
	void SpawnStageUnit();

protected:
	EGameState GameStateType;
	int32 StageID;
	int32 RoundIndex;
	const int32 MaxRound = 3;

	UPROPERTY(transient)
	class UPDStageRoundSpawner* Spawner;
};

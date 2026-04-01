// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../PDGameModeBase.h"
#include "../UI/Battle/PDUIBattleMainWidget.h"
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
	virtual void Tick(float DeltaSeconds) override;

	virtual void RestartPlayer(AController* NewPlayer) override;

	void DespawnUnit(const FGuid& InUnitGuid);
	void UpdatePlayerHP(int32 CurHP, int32 MaxHP);

protected:
	void ChangeGameState(const EGameState InGameState);
	void ReadyGame();
	void StartGame();
	void EndGame();

	void StartStage(const int32 InStageID);
	void SpawnStageUnit();

protected:

	EGameState GameStateType;
	int32 StageID;

	/** 전투 메인 UI 위젯 (경로로 로드, UIManager Panel2D에 추가) */
	UPROPERTY(transient)
	TObjectPtr<UPDUIBattleMainWidget> BattleMainWidget;

	/** WBP_BattleMainUI 블루프린트 경로 */
	static const TCHAR* BattleMainWidgetPath;
};

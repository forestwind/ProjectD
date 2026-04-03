// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../PDGameModeBase.h"
#include "../UI/Battle/PDUIBattleMainWidget.h"
#include "../UI/Battle/PDUIBattlePhaseMsgWidget.h"
#include "../UI/Battle/PDUIBattleEndWidget.h"
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
	void UpdateMonsterCount();

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
	int32 TotalEnemyCount = 0;
	int32 RemainingEnemyCount = 0;

	// 전투 메인 UI
	UPROPERTY(transient)
	TObjectPtr<UPDUIBattleMainWidget> BattleMainWidget;

	// 전투 종료 결과 UI
	UPROPERTY(transient)
	TObjectPtr<UPDUIBattleEndWidget> BattleEndWidget;

	// READY/START 메시지
	UPROPERTY(transient)
	TObjectPtr<UPDUIBattlePhaseMsgWidget> PhaseMsgWidget;

	// WBP_BattleMainUI 블루프린트 경로
	static const TCHAR* BattleMainWidgetPath;
	// WBP_BattleEndUI 블루프린트 경로
	static const TCHAR* BattleEndWidgetPath;
	// WBP_BattlePhaseMsg 블루프린트 경로
	static const TCHAR* PhaseMsgWidgetPath;
	
	// Ready Text 표시 시간
	UPROPERTY(EditDefaultsOnly, Category = "Battle|UI", meta = (ClampMin = "0.0"))
	float ReadyMessageDuration = 3.0f;
	
	// START Text 표시 시간
	UPROPERTY(EditDefaultsOnly, Category = "Battle|UI", meta = (ClampMin = "0.0"))
	float StartMessageDuration = 1.0f;
	
	void ShowUIPhaseMessage(EGameState state);
	void ShowBattleEndUI();
	
private:
	
	FTimerHandle HidePhaseMsgTimerHandle;
	
	void ShowPhaseMessage(const FText& InText);
	void HidePhaseMessage();
};

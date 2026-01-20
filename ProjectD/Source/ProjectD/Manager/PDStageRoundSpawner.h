#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PDStageRoundSpawner.generated.h"

class UModelManager;
class APDCharacter;
class UPDStageDataAsset;
struct FPDStageRow;
struct FPDMonsterGroupRow;
struct FStageUnitSpawnPoint;

USTRUCT(BlueprintType)
struct FPDSpawnedStageUnitResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "PD|Stage|Spawn")
	TObjectPtr<APDCharacter> Character = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "PD|Stage|Spawn")
	int32 UnitId = 0;

	UPROPERTY(BlueprintReadOnly, Category = "PD|Stage|Spawn")
	int32 UnitLv = 0;

	UPROPERTY(BlueprintReadOnly, Category = "PD|Stage|Spawn")
	int32 SlotIndex = 0;

	UPROPERTY(BlueprintReadOnly, Category = "PD|Stage|Spawn")
	FVector SpawnLocation = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly, Category = "PD|Stage|Spawn")
	FRotator SpawnRotation = FRotator::ZeroRotator;
};

/**
 * StageId / RoundIndex 를 입력으로 받아
 * - UPDTableManagerSubsystem 에서 FPDStageRow 조회
 * - StageRow.DataAssetName 으로 UPDStageDataAsset 로드
 * - 라운드 스폰 포인트(좌표/로테이션) 조회
 * - MonsterGroupId 로 유닛ID들을 찾고
 * - 해당 좌표에 APDCharacter 스폰(현재는 UModelManager::SpawnCharacter 사용)
 */
UCLASS(BlueprintType)
class PROJECTD_API UPDStageRoundSpawner : public UObject
{
	GENERATED_BODY()

public:
	// 외부에서 ModelManager 주입 (권장: APDGameModeBase가 생성/보유하는 ModelManager를 넘김)
	UFUNCTION(BlueprintCallable, Category = "PD|Stage|Spawn")
	void Initialize(UModelManager* InModelManager);

	// RoundIndex 는 0-based 를 기준으로 처리 (0->MonsterGroup1, 1->MonsterGroup2, 2->MonsterGroup3)
	UFUNCTION(BlueprintCallable, Category = "PD|Stage|Spawn")
	TArray<FPDSpawnedStageUnitResult> SpawnEnemyStageRoundUnits(int32 StageId, int32 RoundIndex);

	// TODO: 아군 스폰은 추후 파티/덱 데이터 기반으로 구현
	UFUNCTION(BlueprintCallable, Category = "PD|Stage|Spawn")
	TArray<APDCharacter*> SpawnAllyStageRoundUnits(int32 StageId, int32 RoundIndex);

	// UObject 이므로 Outer 체인으로 World 접근 보강
	virtual UWorld* GetWorld() const override;

private:
	UPROPERTY(Transient)
	TObjectPtr<UModelManager> ModelManager;

	static int32 GetMonsterGroupIdByRoundIndex(const FPDStageRow& StageRow, int32 RoundIndex);
	static void CollectUnitIdsFromMonsterGroup(const FPDMonsterGroupRow& GroupRow, TArray<TPair<int32, int32>>& OutUnitIdAndLv);
	static void CollectEnemySpawnPoints(const UPDStageDataAsset& StageAsset, int32 RoundIndex, TArray<FStageUnitSpawnPoint>& OutPoints);
};


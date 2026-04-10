// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ModelManager.generated.h"

class APDCharacter;
class APDBattleItemActor;
/**
 * 
 */
UCLASS()
class PROJECTD_API UModelManager : public UObject
{
	GENERATED_BODY()
	
public:
	UModelManager();
	virtual void BeginDestroy() override;

	APDCharacter* SpawnCharacter(const int32 InUnitID, const int32 InLevel, const FVector& InPosition, const FRotator& InRotation);
	void DespawnCharacter(const FGuid InUnitGuid, bool InImmediately = false);
	APDCharacter* FindCharacter(const FGuid InUnitGuid);
	void AddCharacter(APDCharacter* PDCharacter, const int32 InUnitID, const int32 InLevel);
	int32 GetCharacterCount() const { return CharacterMap.Num(); }

	TMap<FGuid, TObjectPtr<APDCharacter>> GetCharacterMap() const { return CharacterMap; }

	APDBattleItemActor* SpawnBattleItem(int32 ItemID, const FVector& InPosition, const FRotator& InRotation);
	void DespawnBattleItem(const FGuid InItemGuid);
	APDBattleItemActor* FindBattleItem(const FGuid InItemGuid);
	int32 GetBattleItemCount() const { return BattleItemMap.Num(); }

	TMap<FGuid, TObjectPtr<APDBattleItemActor>> GetBattleItemMap() const { return BattleItemMap; }

private:
	UPROPERTY(transient)
	TMap<FGuid, TObjectPtr<APDCharacter>> CharacterMap;

	UPROPERTY(transient)
	TMap<FGuid, TObjectPtr<APDBattleItemActor>> BattleItemMap;
};

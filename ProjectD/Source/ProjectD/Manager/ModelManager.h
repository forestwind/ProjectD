// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ModelManager.generated.h"

class APDCharacter;
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

	APDCharacter* SpawnCharacter(const int32 InUnitID, const FVector& InPosition, const FRotator& InRotation);
	void DespawnCharacter(const FGuid InUnitGuid, bool InImmediately = false);
	APDCharacter* FindCharacter(const FGuid InUnitGuid);

private:
	TMap<FGuid, TObjectPtr<APDCharacter>> CharacterMap;
};

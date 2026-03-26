// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PDBattleSpawnActor.generated.h"

class USceneComponent;
class UArrowComponent;

UENUM(BlueprintType)
enum class EPDBattleSpawnType : uint8
{
	None,
	Enemy,
	Item,
};

UCLASS()
class PROJECTD_API APDBattleSpawnActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APDBattleSpawnActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BattleSpawn")
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BattleSpawn")
	TObjectPtr<UArrowComponent> ArrowComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BattleSpawn")
	int32 BattleSpawnID = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BattleSpawn")
	EPDBattleSpawnType SpawnType = EPDBattleSpawnType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BattleSpawn")
	int32 SpawnTypeID = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BattleSpawn")
	int32 SpawnTypeLevel = 0;
};

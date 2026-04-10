// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PDBattleItemActor.generated.h"

class UStaticMeshComponent;

/**
 * 레벨에 배치되는 배틀 아이템 액터
 */
UCLASS()
class PROJECTD_API APDBattleItemActor : public AActor
{
	GENERATED_BODY()

public:
	APDBattleItemActor();

	void Initialize(const FGuid& InItemGuid, int32 InItemID);

	FGuid GetItemGuid() const { return ItemGuid; }
	int32 GetItemID() const { return ItemID; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BattleItemActor")
	TObjectPtr<UStaticMeshComponent> MeshComponent;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BattleItemActor")
	TObjectPtr<USceneComponent> RootScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BattleItemActor")
	FGuid ItemGuid;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BattleItemActor")
	int32 ItemID = 0;
};

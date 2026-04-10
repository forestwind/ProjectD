// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PDBattleInventorySlot.generated.h"

/** 인벤토리의 칸 하나: 어떤 아이템(ItemID)이 이 칸을 차지하는지, 그 칸에 겹쳐 둔 수량(Count). */
USTRUCT(BlueprintType)
struct PROJECTD_API FPDBattleInventorySlot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Battle|Inventory")
	int32 ItemID = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Battle|Inventory")
	int32 Count = 0;
};

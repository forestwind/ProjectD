// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PDBattleItemRow.generated.h"

UENUM(BlueprintType)
enum class EPDBattleItemEffectType : uint8
{
	None UMETA(DisplayName = "None"),
	HP_Up UMETA(DisplayName = "HP_Up"),
	Attack_Up UMETA(DisplayName = "Attack_Up")
};

USTRUCT(BlueprintType)
struct PROJECTD_API FPDBattleItemRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BattleItem")
	int32 ID = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BattleItem")
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BattleItem")
	EPDBattleItemEffectType EffectType = EPDBattleItemEffectType::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BattleItem")
	int32 EffectValue = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BattleItem")
	int32 MaxStack = 99;

	/** DT_Image의 ImageName과 매칭되는 이미지 식별자 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BattleItem")
	FName ImageName;
};

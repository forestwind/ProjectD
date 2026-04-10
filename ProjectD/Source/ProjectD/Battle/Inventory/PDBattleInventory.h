// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PDBattleInventorySlot.h"
#include "Table/PDTableManagerSubsystem.h"
#include "PDBattleInventory.generated.h"

/**
 * 배틀 소비 아이템 인벤토리. 칸(Slot)마다 아이템 종류 하나와 그 칸에 쌓인 개수를 둔다.
 * 동일 ItemID가 여러 칸으로 나뉠 수 있음(테이블 MaxStack 상한 때문).
 */
UCLASS(BlueprintType)
class PROJECTD_API UPDBattleInventory : public UObject
{
	GENERATED_BODY()

public:
	UPDBattleInventory();

	/** 테이블 조회용 서브시스템과, 인벤에 둘 수 있는 칸(Slot) 개 상한을 한 번에 설정한다. */
	UFUNCTION(BlueprintCallable, Category = "Battle|Inventory")
	void Initialize(UPDTableManagerSubsystem* InTableManager, int32 InMaxSlotCount = 32);

	bool AddItem(int32 ItemID, int32 Amount);

	/** 해당 칸에서 아이템 1개만 소모한다. */
	bool TryConsumeFromSlotIndex(int32 SlotIndex);

	const TArray<FPDBattleInventorySlot>& GetSlots() const { return InventorySlots; }

	int32 GetTotalCountForItem(int32 ItemID) const;

	void Clear();

#if !UE_BUILD_SHIPPING
	void PrintInventorySlotsToLog() const;
#endif

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Battle|Inventory")
	TArray<FPDBattleInventorySlot> InventorySlots;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Battle|Inventory", meta = (ClampMin = "1"))
	int32 MaxSlotCount = 32;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Battle|Inventory")
	TObjectPtr<UPDTableManagerSubsystem> TableManager;
};

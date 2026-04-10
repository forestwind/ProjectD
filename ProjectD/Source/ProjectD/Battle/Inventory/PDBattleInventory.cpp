// Fill out your copyright notice in the Description page of Project Settings.

#include "PDBattleInventory.h"
#include "Table/PDTableManagerSubsystem.h"

UPDBattleInventory::UPDBattleInventory()
{
	MaxSlotCount = 32;
}

void UPDBattleInventory::Initialize(UPDTableManagerSubsystem* InTableManager, int32 InMaxSlotCount)
{
	TableManager = InTableManager;
	MaxSlotCount = FMath::Max(1, InMaxSlotCount);
}

bool UPDBattleInventory::AddItem(int32 ItemID, int32 Amount)
{
	// BattleItem 테이블을 읽기 위해 TableManager가 필요하다.
	if (!TableManager)
	{
		UE_LOG(LogTemp, Log, TEXT("[PD][BattleInventory] AddItem failed: TableManager not set — call Initialize first (ItemID=%d, Amount=%d)"), ItemID, Amount);
		return false;
	}

	if (Amount <= 0)
	{
		UE_LOG(LogTemp, Log, TEXT("[PD][BattleInventory] AddItem failed: Amount <= 0 (ItemID=%d, Amount=%d)"), ItemID, Amount);
		return false;
	}

	const FPDBattleItemRow* BattleItemRow = TableManager->GetBattleItem(ItemID);
	if (!BattleItemRow)
	{
		UE_LOG(LogTemp, Log, TEXT("[PD][BattleInventory] AddItem failed: unknown ItemID %d"), ItemID);
		return false;
	}

	// 칸 하나에 같은 아이템을 최대 몇 개까지 겹쳐 둘 수 있는지
	const int32 MaxCountPerSlot = FMath::Max(1, BattleItemRow->MaxStack);

	// 실패 시 기존 InventorySlots 를 건드리지 않도록, 복사본에서만 계산한 뒤 성공 시에만 대입한다.
	TArray<FPDBattleInventorySlot> PendingSlots = InventorySlots;
	// 아직 어느 칸에도 배치하지 않은 남은 개수
	int32 AmountStillToPlace = Amount;

	// 같은 ItemID 를 담고 있는 기존 칸을 앞에서부터 찾아, 한 칸이 가득 찰 때까지 남은 개수를 채운다.
	for (int32 SlotIndex = 0; SlotIndex < PendingSlots.Num(); ++SlotIndex)
	{
		if (AmountStillToPlace <= 0)
		{
			break;
		}

		if (PendingSlots[SlotIndex].ItemID != ItemID)
		{
			continue;
		}

		// 이 칸에 더 넣을 수 있는 개수
		const int32 RemainingCapacityInSlot = MaxCountPerSlot - PendingSlots[SlotIndex].Count;
		if (RemainingCapacityInSlot <= 0)
		{
			continue;
		}

		const int32 AddedToThisSlot = FMath::Min(RemainingCapacityInSlot, AmountStillToPlace);
		PendingSlots[SlotIndex].Count += AddedToThisSlot;
		AmountStillToPlace -= AddedToThisSlot;
	}

	// 남은 개수가 있으면 "새 칸"을 뒤에 만든다. 칸 하나에는 최대 MaxCountPerSlot 개까지.
	while (AmountStillToPlace > 0)
	{
		// 인벤 전체 칸 수가 MaxSlotCount 를 넘으면 더 이상 칸을 만들 수 없음 → 이번 Add 전체 실패
		if (PendingSlots.Num() >= MaxSlotCount)
		{
			UE_LOG(LogTemp, Log, TEXT("[PD][BattleInventory] AddItem failed: MaxSlotCount %d reached (ItemID=%d, remaining=%d)"),
				MaxSlotCount, ItemID, AmountStillToPlace);
			return false;
		}

		// 새 칸에 넣을 개수: 남은 양과 칸당 상한 중 작은 값
		const int32 CountForNewSlot = FMath::Min(MaxCountPerSlot, AmountStillToPlace);
		FPDBattleInventorySlot NewSlot;
		NewSlot.ItemID = ItemID;
		NewSlot.Count = CountForNewSlot;
		PendingSlots.Add(MoveTemp(NewSlot));
		AmountStillToPlace -= CountForNewSlot;
	}

	InventorySlots = MoveTemp(PendingSlots);
	return true;
}

bool UPDBattleInventory::TryConsumeFromSlotIndex(int32 SlotIndex)
{
	if (!InventorySlots.IsValidIndex(SlotIndex))
	{
		UE_LOG(LogTemp, Log, TEXT("[PD][BattleInventory] TryConsume failed: invalid SlotIndex %d (Num=%d)"), SlotIndex, InventorySlots.Num());
		return false;
	}

	if (InventorySlots[SlotIndex].Count < 1)
	{
		UE_LOG(LogTemp, Log, TEXT("[PD][BattleInventory] TryConsume failed: empty slot (SlotIndex=%d)"), SlotIndex);
		return false;
	}

	--InventorySlots[SlotIndex].Count;
	if (InventorySlots[SlotIndex].Count <= 0)
	{
		InventorySlots.RemoveAt(SlotIndex);
	}

	return true;
}

int32 UPDBattleInventory::GetTotalCountForItem(int32 ItemID) const
{
	int32 Total = 0;
	for (const FPDBattleInventorySlot& Slot : InventorySlots)
	{
		if (Slot.ItemID == ItemID)
		{
			Total += Slot.Count;
		}
	}
	return Total;
}

void UPDBattleInventory::Clear()
{
	InventorySlots.Reset();
}

#if !UE_BUILD_SHIPPING
void UPDBattleInventory::PrintInventorySlotsToLog() const
{
	UE_LOG(LogTemp, Log, TEXT("[PD][BattleInventory] InventorySlots (%d slots, MaxSlotCount=%d):"), InventorySlots.Num(), MaxSlotCount);
	for (int32 i = 0; i < InventorySlots.Num(); ++i)
	{
		UE_LOG(LogTemp, Log, TEXT("[PD][BattleInventory]  [slot %d] ItemID=%d Count=%d"), i, InventorySlots[i].ItemID, InventorySlots[i].Count);
	}
}
#endif

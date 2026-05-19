// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Battle/Inventory/PDBattleInventorySlot.h"
#include "PDUIBattleInventoryScrollItemWidget.generated.h"

class UImage;
class UTextBlock;
class UPDTableManagerSubsystem;

/**
 * 인벤토리 스크롤 목록의 행 하나. BP에서 위젯 이름을 맞춰야 바인딩됩니다.
 */
UCLASS()
class PROJECTD_API UPDUIBattleInventoryScrollItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/** 슬롯 데이터를 받아 위젯 표시를 갱신합니다. 빈 슬롯(ItemID=0)이면 아이콘·수량을 숨깁니다. */
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetSlotData(const FPDBattleInventorySlot& InSlot);

protected:
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UImage> ItemIcon;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> TextItemCount;
};
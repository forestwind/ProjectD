// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Battle/Inventory/PDBattleInventorySlot.h"
#include "PDUIBattleInventoryScrollItemWidget.generated.h"

class UImage;
class UTextBlock;
class UButton;
class UPDTableManagerSubsystem;

/**
 * 인벤토리 스크롤 목록의 행 하나. BP에서 위젯 이름을 맞춰야 바인딩됩니다.
 */
UCLASS()
class PROJECTD_API UPDUIBattleInventoryScrollItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/** 슬롯 데이터와 인벤토리 인덱스를 받아 위젯 표시를 갱신합니다. */
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetSlotData(const FPDBattleInventorySlot& InSlot, int32 InSlotIndex);

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnUseButtonClicked();

protected:
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UImage> ItemIcon;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> TextItemCount;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UButton> UseButton;

private:
	int32 SlotIndex = -1;
};
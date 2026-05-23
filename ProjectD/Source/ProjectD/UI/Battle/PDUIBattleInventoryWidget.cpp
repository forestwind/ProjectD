// Fill out your copyright notice in the Description page of Project Settings.

#include "PDUIBattleInventoryWidget.h"
#include "PDUIBattleInventoryScrollItemWidget.h"
#include "Components/ScrollBox.h"
#include "Components/WrapBox.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Battle/PDBattleGameMode.h"

void UPDUIBattleInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (CloseButton)
	{
		CloseButton->OnClicked.AddDynamic(this, &UPDUIBattleInventoryWidget::OnCloseButtonClicked);
	}
}

void UPDUIBattleInventoryWidget::OnCloseButtonClicked()
{
	if (APDBattleGameMode* BattleGM = Cast<APDBattleGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		BattleGM->ToggleBattleInventory();
	}
}

void UPDUIBattleInventoryWidget::AddItem(const FPDBattleInventorySlot& InSlot, int32 SlotIndex)
{
	if (!ItemWrapBox || !ItemRowClass)
	{
		return;
	}

	UPDUIBattleInventoryScrollItemWidget* Row =
		CreateWidget<UPDUIBattleInventoryScrollItemWidget>(this, ItemRowClass);
	if (Row)
	{
		Row->SetSlotData(InSlot, SlotIndex);
		ItemWrapBox->AddChild(Row);
	}
}

void UPDUIBattleInventoryWidget::ClearItems()
{
	if (ItemWrapBox)
	{
		ItemWrapBox->ClearChildren();
	}
}
// Fill out your copyright notice in the Description page of Project Settings.

#include "PDUIBattleInventoryWidget.h"
#include "PDUIBattleInventoryScrollItemWidget.h"
#include "Components/ScrollBox.h"
#include "Components/WrapBox.h"

void UPDUIBattleInventoryWidget::AddItem(const FPDBattleInventorySlot& InSlot)
{
	if (!ItemWrapBox || !ItemRowClass)
	{
		return;
	}

	UPDUIBattleInventoryScrollItemWidget* Row =
		CreateWidget<UPDUIBattleInventoryScrollItemWidget>(this, ItemRowClass);
	if (Row)
	{
		Row->SetSlotData(InSlot);
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
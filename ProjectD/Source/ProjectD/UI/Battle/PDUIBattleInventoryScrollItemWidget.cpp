// Fill out your copyright notice in the Description page of Project Settings.

#include "PDUIBattleInventoryScrollItemWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UPDUIBattleInventoryScrollItemWidget::SetSlotData(const FPDBattleInventorySlot& InSlot)
{
	const bool bEmpty = (InSlot.ItemID == 0);

	if (ItemIcon)
	{
		ItemIcon->SetVisibility(bEmpty ? ESlateVisibility::Hidden : ESlateVisibility::SelfHitTestInvisible);
	}

	if (TextItemCount)
	{
		if (bEmpty)
		{
			TextItemCount->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			TextItemCount->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			TextItemCount->SetText(FText::FromString(FString::Printf(TEXT("x%d"), InSlot.Count)));
		}
	}
}
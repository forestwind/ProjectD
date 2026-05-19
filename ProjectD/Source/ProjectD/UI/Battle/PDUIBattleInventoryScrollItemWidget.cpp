// Fill out your copyright notice in the Description page of Project Settings.

#include "PDUIBattleInventoryScrollItemWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Table/PDTableManagerSubsystem.h"
#include "Table/PDBattleItemRow.h"

void UPDUIBattleInventoryScrollItemWidget::SetSlotData(const FPDBattleInventorySlot& InSlot)
{
	const bool bEmpty = (InSlot.ItemID == 0);

	if (ItemIcon)
	{
		if (bEmpty)
		{
			ItemIcon->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			UTexture2D* IconTexture = nullptr;

			if (UGameInstance* GI = UGameplayStatics::GetGameInstance(this))
			{
				if (UPDTableManagerSubsystem* TableManager = GI->GetSubsystem<UPDTableManagerSubsystem>())
				{
					if (const FPDBattleItemRow* ItemRow = TableManager->GetBattleItem(InSlot.ItemID))
					{
						IconTexture = TableManager->GetImage(ItemRow->ImageName);
					}
				}
			}

			if (IconTexture)
			{
				ItemIcon->SetBrushFromTexture(IconTexture);
			}
			ItemIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}

	if (TextItemCount)
	{
		if (bEmpty)
		{
			TextItemCount->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			TextItemCount->SetText(FText::FromString(FString::Printf(TEXT("x%d"), InSlot.Count)));
			TextItemCount->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}
}
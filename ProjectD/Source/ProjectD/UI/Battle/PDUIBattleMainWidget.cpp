// Fill out your copyright notice in the Description page of Project Settings.


#include "PDUIBattleMainWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UPDUIBattleMainWidget::UpdateMonsterCount(int32 Remaining, int32 Total)
{
	if (TextMonsterCount)
	{
		TextMonsterCount->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), Remaining, Total)));
	}
}

void UPDUIBattleMainWidget::UpdatePlayerHP(int32 CurHP, int32 MaxHP)
{
	if (PbPlayerHp)
	{
		const float Percent = MaxHP > 0 ? FMath::Clamp((float)CurHP / (float)MaxHP, 0.0f, 1.0f) : 0.0f;
		PbPlayerHp->SetPercent(Percent);
	}

	if (TextPlayerHp)
	{
		TextPlayerHp->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"), CurHP, MaxHP)));
	}
}


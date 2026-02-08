// Fill out your copyright notice in the Description page of Project Settings.


#include "PDUIBattleHpBarWidget.h"

void UPDUIBattleHpBarWidget::UpdateHpBar(float Value)
{
	if (PbHpBar)
	{
		PbHpBar->SetPercent(FMath::Clamp(Value, 0.f, 1.f));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PbHpBar is not bound in UPDUIBattleHpBarWidget"));
	}
}

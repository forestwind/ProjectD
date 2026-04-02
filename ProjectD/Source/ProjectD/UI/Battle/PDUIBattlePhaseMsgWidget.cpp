// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Battle/PDUIBattlePhaseMsgWidget.h"
#include "Components/TextBlock.h"

void UPDUIBattlePhaseMsgWidget::SetPhaseText(const FText& InText)
{
	if (TextPhase)
	{
		TextPhase->SetText(InText);
	}
}

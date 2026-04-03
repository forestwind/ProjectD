// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Battle/PDUIBattleEndWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UPDUIBattleEndWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (BtnRetry)
	{
		BtnRetry->OnClicked.AddDynamic(this, &UPDUIBattleEndWidget::OnRetryClicked);
	}

	if (BtnTitle)
	{
		BtnTitle->OnClicked.AddDynamic(this, &UPDUIBattleEndWidget::OnTitleClicked);
	}
}

void UPDUIBattleEndWidget::SetResultText(const FText& InText)
{
	if (TextResult)
	{
		TextResult->SetText(InText);
	}
}

void UPDUIBattleEndWidget::OnRetryClicked()
{
	// TODO: 배틀 재시작 처리
}

void UPDUIBattleEndWidget::OnTitleClicked()
{
	// TODO: 타이틀로 이동 처리
}
// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Battle/PDUIBattleEndWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Core/PDUIManagerSubsystem.h"

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
	if (UPDUIManagerSubsystem* UIManager = GetGameInstance()->GetSubsystem<UPDUIManagerSubsystem>())
	{
		UIManager->ClearPanel2D();
		UIManager->ClearPanelOverlay();
	}
	UGameplayStatics::OpenLevel(this, TEXT("ForestMap"));
}

void UPDUIBattleEndWidget::OnTitleClicked()
{
	if (UPDUIManagerSubsystem* UIManager = GetGameInstance()->GetSubsystem<UPDUIManagerSubsystem>())
	{
		UIManager->ClearPanel2D();
		UIManager->ClearPanelOverlay();
	}
	UGameplayStatics::OpenLevel(this, TEXT("Title"));
}
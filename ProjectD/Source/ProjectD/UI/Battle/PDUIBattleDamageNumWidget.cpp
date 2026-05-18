// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Battle/PDUIBattleDamageNumWidget.h"

#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"
#include "UI/Core/PDUIManagerSubsystem.h"

void UPDUIBattleDamageNumWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ElapsedTime = 0.f;
	bIsActive = false;
	SetRenderOpacity(0.f);
}

void UPDUIBattleDamageNumWidget::ShowDamage(float DamageAmount)
{
	DisplayDuration = FMath::Max(DisplayDuration, 0.1f);
	ElapsedTime = 0.f;
	bIsActive = true;
	SetRenderOpacity(1.f);

	if (TextDamage)
	{
		TextDamage->SetText(FText::AsNumber(FMath::RoundToInt(DamageAmount)));
	}
}

void UPDUIBattleDamageNumWidget::NativeTick(const FGeometry& AllottedGeometry, float InDeltaTime)
{
	Super::NativeTick(AllottedGeometry, InDeltaTime);

	if (!bIsActive)
	{
		return;
	}

	ElapsedTime += InDeltaTime;

	// 위로 이동
	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Slot);
	if (CanvasSlot)
	{
		const FVector2D CurrentPos = CanvasSlot->GetPosition();
		CanvasSlot->SetPosition(CurrentPos - FVector2D(0.f, RiseSpeed * InDeltaTime));
	}

	// 페이드 아웃
	if (DisplayDuration > 0.f)
	{
		SetRenderOpacity(FMath::Clamp(1.f - (ElapsedTime / DisplayDuration), 0.f, 1.f));
	}

	// 시간 만료 시 자기 제거
	if (ElapsedTime >= DisplayDuration)
	{
		UGameInstance* GI = GetGameInstance();
		if (!GI)
		{
			return;
		}

		UPDUIManagerSubsystem* UIManager = GI->GetSubsystem<UPDUIManagerSubsystem>();
		if (!UIManager)
		{
			return;
		}

		UIManager->RemoveWidget(this);
	}
}
// Fill out your copyright notice in the Description page of Project Settings.

#include "PDHpBarWidgetComponent.h"
#include "PDUIBattleHpBarWidget.h"
#include "../../Character/PDCharacter.h"

UPDHpBarWidgetComponent::UPDHpBarWidgetComponent()
{
	SetDrawSize(FVector2D(200.f, 30.f));
	SetRelativeLocation(FVector(0.f, 0.f, 100.f));
	SetWidgetSpace(EWidgetSpace::Screen);
	SetInitialLayerZOrder(100);
}

void UPDHpBarWidgetComponent::BeginPlay()
{
	Super::BeginPlay();
	ResolveAndSetWidgetClass();
	UpdateHpBar(1.0f);
}

void UPDHpBarWidgetComponent::UpdateHpBar(float Value)
{
	UUserWidget* CurrentWidget = GetWidget();
	UPDUIBattleHpBarWidget* HpBarWidget = Cast<UPDUIBattleHpBarWidget>(CurrentWidget);
	if (HpBarWidget)
	{
		HpBarWidget->UpdateHpBar(Value);
	}
}

void UPDHpBarWidgetComponent::ResolveAndSetWidgetClass()
{
	UClass* ResolvedClass = HpBarWidgetClass ? HpBarWidgetClass.Get() : nullptr;
	if (!ResolvedClass)
	{
		ResolvedClass = LoadClass<UUserWidget>(nullptr, TEXT("/Game/UI/Battle/WBP_BattleHpBar.WBP_BattleHpBar_C"));
	}
	if (!ResolvedClass)
	{
		ResolvedClass = UPDUIBattleHpBarWidget::StaticClass();
	}
	SetWidgetClass(ResolvedClass);
}

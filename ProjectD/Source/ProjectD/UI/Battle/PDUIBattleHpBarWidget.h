// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "PDUIBattleHpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API UPDUIBattleHpBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// 0~1 비율로 HP 바 갱신
	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateHpBar(float Value);

protected:
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UProgressBar> PbHpBar;
};

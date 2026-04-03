// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PDUIBattleEndWidget.generated.h"

class UTextBlock;
class UButton;

/**
 * 전투 종료 결과 위젯
 */
UCLASS()
class PROJECTD_API UPDUIBattleEndWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/** 결과 텍스트 설정 (예: "Victory", "Defeat") */
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetResultText(const FText& InText);

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> TextResult;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UButton> BtnRetry;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UButton> BtnTitle;

	UFUNCTION()
	void OnRetryClicked();

	UFUNCTION()
	void OnTitleClicked();
};
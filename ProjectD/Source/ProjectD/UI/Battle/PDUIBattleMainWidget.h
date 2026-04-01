// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PDUIBattleMainWidget.generated.h"

class UProgressBar;
class UTextBlock;

/**
 *
 */
UCLASS()
class PROJECTD_API UPDUIBattleMainWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdatePlayerHP(int32 CurHP, int32 MaxHP);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> PbPlayerHp;
	
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> TextPlayerHp;
};

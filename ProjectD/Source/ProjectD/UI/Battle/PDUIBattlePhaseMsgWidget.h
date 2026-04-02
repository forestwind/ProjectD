// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PDUIBattlePhaseMsgWidget.generated.h"

class FText;
class UTextBlock;
/**
 * 
 */
UCLASS()
class PROJECTD_API UPDUIBattlePhaseMsgWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetPhaseText(const FText& InText);
	
protected:
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> TextPhase;
};

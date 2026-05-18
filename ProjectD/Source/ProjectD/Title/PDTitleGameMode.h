// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../PDGameModeBase.h"
#include "InputActionValue.h"
#include "PDTitleGameMode.generated.h"

class UInputMappingContext;
class UInputAction;
class UPDTitleMainWidget;

/**
 * 
 */
UCLASS()
class PROJECTD_API APDTitleGameMode : public APDGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

protected:
	void SetupTitleUI();
	void SetupEnhancedInput();
	void StartBattleLevel();
	void HandleTapInput(const FInputActionValue& Value);
	void HandleStartRequestedFromWidget();

	UPROPERTY(Transient)
	TObjectPtr<UPDTitleMainWidget> TitleWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> TitleMappingContext; // IMC_Title

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> TapAction; // IA_Tap


	UPROPERTY(EditDefaultsOnly, Category = "Title")
	FName BattleLevelName = TEXT("Battle");
};

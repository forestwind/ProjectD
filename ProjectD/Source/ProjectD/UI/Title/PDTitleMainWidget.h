// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PDTitleMainWidget.generated.h"


class UButton;

DECLARE_MULTICAST_DELEGATE(FOnStartRequested);
/**
 * 
 */
UCLASS()
class PROJECTD_API UPDTitleMainWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	FOnStartRequested OnStartRequested;

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Btn_Background;

	UFUNCTION()
	void HandleBackgroundClicked();
};

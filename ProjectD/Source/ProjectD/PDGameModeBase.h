// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PDGameModeBase.generated.h"

class UModelManager;

/**
 * 
 */
UCLASS(Abstract)
class PROJECTD_API APDGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	APDGameModeBase();
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void BeginDestroy() override;

protected:
	TObjectPtr<UModelManager> ModelManager;
};

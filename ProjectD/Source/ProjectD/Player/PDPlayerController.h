// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PDPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API APDPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void OnPossess(APawn* aPawn) override;

	int32 GetPlayerUnitID() const { return PlayerUnitID; }

protected:
	int32 PlayerUnitID = 1;
};

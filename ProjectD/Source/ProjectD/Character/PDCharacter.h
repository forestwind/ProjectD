// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PDCharacter.generated.h"

//class FUnitInfo
//{
//
//};


UCLASS()
class PROJECTD_API APDCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APDCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// TableManager DataAsset TestCode
	void TestLogUnitDataAsset_Unit(int32 UnitTableID);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	FGuid UnitGuid;
};

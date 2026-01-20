// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PDCharacter.generated.h"

class UPDUnitDataAsset;
enum class EAIState : uint8;

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

	FGuid GetUnitGuid() const { return UnitGuid; }
	void SetUnitGuid(const FGuid InUnitGuid) { UnitGuid = InUnitGuid; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// TableManager DataAsset TestCode
	void LoadInfo(const int32 UnitTableID);
	void TestLogStageDataAsset_Stage(const FString& StageAssetName);

	void LoadAnimation();
	void ChangeAnimation(EAIState InAIState);
	void AnimationEnd(UAnimMontage* InMontage, bool bInterrupted);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY()
	UPDUnitDataAsset* UnitAsset;

	UPROPERTY()
	UAnimMontage* IdleMontage;

	UPROPERTY()
	UAnimMontage* AttackMontage;

	UPROPERTY()
	UAnimMontage* DieMontage;

	UPROPERTY()
	UAnimMontage* VictoryMontage;

protected:
	FGuid UnitGuid;
	int32 UnitID;
};

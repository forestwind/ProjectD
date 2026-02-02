// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PDCharacter.generated.h"

struct FPDUnitRow;
class UPDTableManagerSubsystem;
class UPDUnitDataAsset;
enum class EAIState : uint8;

class FUnitInfo
{
public:
	int32 Level = 1;
	int32 CurHP;
	int32 MaxHP;
	float Attack;
	float Defense;
	int32 Exp;
	
public:
	float GetHPPercent() const {
		return MaxHP > 0 ? (float)CurHP / (float)MaxHP : 0.0f;
	}
};


UCLASS()
class PROJECTD_API APDCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APDCharacter();

	FGuid GetUnitGuid() const { return UnitGuid; }
	//void SetUnitGuid(const FGuid InUnitGuid) { UnitGuid = InUnitGuid; }

	void SetInfo(const int32 InUnitTableID, const FGuid& InUnitGuid);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void LoadInfo(const int32 UnitTableID);
	void LoadStat(const UPDTableManagerSubsystem* InTableManager, const FPDUnitRow* InUnitTable);

	void LoadAnimation();
	void ChangeAnimation(EAIState InAIState);
	void AnimationEnd(UAnimMontage* InMontage, bool bInterrupted);
	void ChangeAIState(EAIState InAIState);

	void Attack();
	void TakeDamaged(const float InDamage);

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
	UAnimMontage* DamagedMontage;

	UPROPERTY()
	UAnimMontage* DieMontage;

	UPROPERTY()
	UAnimMontage* VictoryMontage;

protected:
	FGuid UnitGuid;
	int32 UnitID;

	FUnitInfo UnitInfo;
};

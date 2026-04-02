// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PDCharacter.generated.h"

struct FPDUnitRow;
class UPDTableManagerSubsystem;
class UPDUnitDataAsset;
class UPDHpBarWidgetComponent;
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

	void SetInfo(const int32 InUnitTableID, const int32 InLevel, const FGuid& InUnitGuid);

	void UpdateUIHpBar(float Value);
	float GetHpPercent() const { return UnitInfo.GetHPPercent(); }

	void Attack();
	void CheckAttack();
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	void TakeDamaged(const float InDamage);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void LoadInfo(const int32 UnitTableID);
	void LoadStat(const UPDTableManagerSubsystem* InTableManager, const FPDUnitRow* InUnitTable);

	void LoadAnimation();
	void ChangeAnimation(EAIState InAIState);
	void AnimationEnd(UAnimMontage* InMontage, bool bInterrupted);
	void ChangeAIState(EAIState InAIState);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY()
	UPDUnitDataAsset* UnitAsset;

	UPROPERTY()
	TObjectPtr<UAnimMontage> IdleMontage;

	UPROPERTY()
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY()
	TObjectPtr<UAnimMontage> DamagedMontage;

	UPROPERTY()
	TObjectPtr<UAnimMontage> DieMontage;

	UPROPERTY()
	TObjectPtr<UAnimMontage> MoveMontage;

	UPROPERTY()
	TObjectPtr<UAnimMontage> VictoryMontage;

	/** 캐릭터 머리 위 HP 바 (생성/클래스 로드/갱신은 컴포넌트에서 담당) */
	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<UPDHpBarWidgetComponent> HpBarWidgetComponent;

protected:
	FGuid UnitGuid;
	int32 UnitID;

	FUnitInfo UnitInfo;
};

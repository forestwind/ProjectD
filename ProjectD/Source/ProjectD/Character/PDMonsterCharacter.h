// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PDCharacter.h"
#include "PDMonsterCharacter.generated.h"

DECLARE_DELEGATE(FAICharacterAttackFinished);

/**
 * 
 */
UCLASS()
class PROJECTD_API APDMonsterCharacter : public APDCharacter
{
	GENERATED_BODY()
	
public:
	APDMonsterCharacter();

	virtual void PostInitializeComponents() override;

	// AI
	float GetAIPatrolRadius();
	float GetAIDetectRange();
	float GetAIAttackRange();
	float GetAITurnSpeed();

	void SetAIAttackDelegate(const FAICharacterAttackFinished& InDelegate);
	void AttackByAI();

protected:
	virtual void NotifyAttackEnd() override;
	virtual void SetDeath() override;
	virtual void ChangeAIState(EAIState InAIState) override;


	FAICharacterAttackFinished OnAttackFinished;
};

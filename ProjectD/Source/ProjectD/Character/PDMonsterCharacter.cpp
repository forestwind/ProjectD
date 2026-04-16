// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PDMonsterCharacter.h"
#include "PDAIController.h"

APDMonsterCharacter::APDMonsterCharacter()
{
	AIControllerClass = APDAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void APDMonsterCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

float APDMonsterCharacter::GetAIPatrolRadius()
{
	return 500.0f;
}

float APDMonsterCharacter::GetAIDetectRange()
{
	return 400.0f;
}

float APDMonsterCharacter::GetAIAttackRange()
{
	return 150.0f;
}

float APDMonsterCharacter::GetAITurnSpeed()
{
	return 2.0f;
}

void APDMonsterCharacter::SetAIAttackDelegate(const FAICharacterAttackFinished& InDelegate)
{
	OnAttackFinished = InDelegate;
}

void APDMonsterCharacter::AttackByAI()
{
	Attack();
}

void APDMonsterCharacter::NotifyAttackEnd()
{
	Super::NotifyAttackEnd();
	OnAttackFinished.ExecuteIfBound();
}

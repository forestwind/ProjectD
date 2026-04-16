// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Attack.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/PDMonsterCharacter.h"

UBTTask_Attack::UBTTask_Attack()
{
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	APDMonsterCharacter* MonsterCharacter = Cast<APDMonsterCharacter>(ControllingPawn);
	if (MonsterCharacter == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	FAICharacterAttackFinished OnAttackFinished;
	OnAttackFinished.BindLambda([&]() {
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	});
	MonsterCharacter->SetAIAttackDelegate(OnAttackFinished);
	MonsterCharacter->AttackByAI();
	return EBTNodeResult::InProgress;
}

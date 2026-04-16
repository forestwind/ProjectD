// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_AttackInRange.h"
#include "AI/PDAI.h"
#include "AIController.h"
#include "Character/PDMonsterCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_AttackInRange::UBTDecorator_AttackInRange()
{
	NodeName = TEXT("CanAttack");
}

bool UBTDecorator_AttackInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
	{
		return false;
	}

	APDMonsterCharacter* MonsterCharacter = Cast<APDMonsterCharacter>(ControllingPawn);
	if (MonsterCharacter == nullptr)
	{
		return false;
	}

	APawn* TargetPawn = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	if (TargetPawn == nullptr)
	{
		return false;
	}

	float DistanceToTarget = ControllingPawn->GetDistanceTo(TargetPawn);
	float AttackRangeWithRadius = MonsterCharacter->GetAIAttackRange();
	bResult = (DistanceToTarget <= AttackRangeWithRadius);
	return bResult;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "PDAIController.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AI/PDAI.h"

APDAIController::APDAIController()
{
	CurAIState = EAIState::Ready;

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BlackboardDataObject(TEXT("/Script/AIModule.BlackboardData'/Game/AI/BB_Monster.BB_Monster'"));
	if (BlackboardDataObject.Succeeded())
	{
		BlackboardData = BlackboardDataObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BehaviorTreeObject(TEXT("/Script/AIModule.BehaviorTree'/Game/AI/BT_Monster.BT_Monster'"));
	if (BehaviorTreeObject.Succeeded())
	{
		BehaviorTree = BehaviorTreeObject.Object;
	}
}

void APDAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UpdateAIState();
}

void APDAIController::ChangeAIState(EAIState InNewState)
{
	if (CurAIState == EAIState::Die)
	{
		return;
	}

	LeaveAIState(InNewState);
	SetAIState(InNewState);
	EnterAIState(InNewState);
}

void APDAIController::RunAI()
{
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	if(UseBlackboard(BlackboardData, BlackboardPtr))
	{
		Blackboard->SetValueAsVector(BBKEY_HOMPOS, GetPawn()->GetActorLocation());

		bool bResult = RunBehaviorTree(BehaviorTree);
		ensure(bResult);
	}
}

void APDAIController::StopAI()
{
	UBehaviorTreeComponent* BehaviorTreeComp = Cast<UBehaviorTreeComponent>(BrainComponent);
	if(BehaviorTreeComp)
	{
		BehaviorTreeComp->StopTree();
	}
}

void APDAIController::UpdateAIState()
{

}

void APDAIController::EnterAIState(EAIState InNewState)
{

}

void APDAIController::LeaveAIState(EAIState InNewState)
{

}

void APDAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RunAI();
}

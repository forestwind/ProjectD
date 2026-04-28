// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PDAIController.generated.h"

enum class EAIState : uint8;

/**
 * 
 */
UCLASS()
class PROJECTD_API APDAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	APDAIController();

	virtual void Tick(float DeltaTime) override;

	void ChangeAIState(EAIState InNewState);

	void SetAIState(EAIState InNewState) { CurAIState = InNewState; }
	EAIState GetAIState() const { return CurAIState; }

	void RunAI();
	void StopAI();

protected:
	void UpdateAIState();
	void EnterAIState(EAIState InNewState);
	void LeaveAIState(EAIState InNewState);

protected:
	virtual void OnPossess(APawn* InPawn) override;

protected:
	EAIState CurAIState;

private:
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BlackboardData;

	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BehaviorTree;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PDAIController.generated.h"

UENUM()
enum class EAIState : uint8
{
	Ready,
	Idle,
	Move,
	Attack,
	Die,
	Victory,
	Max
};

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

protected:
	void UpdateAIState();
	void EnterAIState(EAIState InNewState);
	void LeaveAIState(EAIState InNewState);

protected:
	EAIState CurAIState;
};

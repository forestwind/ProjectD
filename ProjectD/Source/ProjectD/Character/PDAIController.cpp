// Fill out your copyright notice in the Description page of Project Settings.


#include "PDAIController.h"


APDAIController::APDAIController()
{

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

void APDAIController::UpdateAIState()
{

}

void APDAIController::EnterAIState(EAIState InNewState)
{

}

void APDAIController::LeaveAIState(EAIState InNewState)
{

}

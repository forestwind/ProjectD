// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PDAnimInstance.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


UPDAnimInstance::UPDAnimInstance()
{
	MovingThreshould = 3.0f;
}

void UPDAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ACharacter>(GetOwningActor());
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void UPDAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement)
	{
		Velocity = Movement->Velocity;
		GroundSpeed = Velocity.Size2D();
		bIsIdle = GroundSpeed < MovingThreshould;
	}
}


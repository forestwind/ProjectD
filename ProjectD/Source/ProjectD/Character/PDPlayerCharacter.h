// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PDCharacter.h"
#include "InputActionValue.h"
#include "PDPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

class UInputAction;
class UInputMappingContext;
/**
 * 
 */
UCLASS()
class PROJECTD_API APDPlayerCharacter : public APDCharacter
{
	GENERATED_BODY()

	APDPlayerCharacter();

protected:

	virtual void PossessedBy(AController* NewController) override;

	// 플레이어 전용 카메라 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> FollowCamera;


	// Enhanced Input 에셋
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> MoveMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> AttackAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> InventoryAction;

protected:

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void HandleMove(const FInputActionValue& Value);
	void HandleAttack();
	void HandleInventory();

	void ApplyCameraSettings();

public:
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	                         AController* EventInstigator, AActor* DamageCauser) override;
};

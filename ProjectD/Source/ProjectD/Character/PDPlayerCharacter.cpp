// Fill out your copyright notice in the Description page of Project Settings.


#include "PDPlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "GameFramework/PlayerController.h"
#include "Battle/PDBattleGameMode.h"


// Sets default values
APDPlayerCharacter::APDPlayerCharacter()
{
	// 플레이어용 캐릭터는 AI 컨트롤러 기본값을 끔
	AIControllerClass = nullptr;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	
	// 3인칭 이동 기본 세팅
	bUseControllerRotationYaw = false;
	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->bOrientRotationToMovement = true;
		MoveComp->RotationRate = FRotator(0.f, 720.f, 0.f);
	}


	static const TCHAR* MoveIMCPath =
		TEXT("/Game/Input/IMC_BattlePlayer.IMC_BattlePlayer");
	static const TCHAR* MoveActionPath =
		TEXT("/Game/Input/IA_BattlePlayerMove.IA_BattlePlayerMove");
	static const TCHAR* AttackActionPath =
		TEXT("/Game/Input/IA_BattlePlayerAttack.IA_BattlePlayerAttack");
	static const TCHAR* InventoryActionPath =
		TEXT("/Game/Input/IA_BattleInventory.IA_BattleInventory");

	MoveMappingContext = LoadObject<UInputMappingContext>(nullptr, MoveIMCPath);
	MoveAction = LoadObject<UInputAction>(nullptr, MoveActionPath);
	AttackAction = LoadObject<UInputAction>(nullptr, AttackActionPath);
	InventoryAction = LoadObject<UInputAction>(nullptr, InventoryActionPath);
}

void APDPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ApplyCameraSettings();
}

void APDPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (!PlayerInputComponent) return;

	if (MoveMappingContext)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		if (PC)
		{
			if (ULocalPlayer* LP = PC->GetLocalPlayer())
			{
				if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
				{
					Subsystem->RemoveMappingContext(MoveMappingContext);
					Subsystem->AddMappingContext(MoveMappingContext, 0);
				}
			}
		}
	}

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MoveAction)
		{
			// 연속 입력: Triggered 사용
			EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APDPlayerCharacter::HandleMove);
		}

		if (AttackAction)
		{
			// 연속 입력: Triggered 사용
			EIC->BindAction(AttackAction, ETriggerEvent::Triggered, this, &APDPlayerCharacter::HandleAttack);
		}

		if (InventoryAction)
		{
			EIC->BindAction(InventoryAction, ETriggerEvent::Started, this, &APDPlayerCharacter::HandleInventory);
		}
	}
}

void APDPlayerCharacter::HandleMove(const FInputActionValue& Value)
{
	if(CurAIState == EAIState::Die)
	{
		return;
	}

	const FVector2D Axis = Value.Get<FVector2D>();
	if (Axis.IsNearlyZero()) return;

	// 쿼터뷰 고정 카메라 기준으로 화면 위/아래, 좌/우를 월드 평면 방향으로 변환한다.
	const FVector CameraForward = FollowCamera ? FollowCamera->GetForwardVector() : GetActorForwardVector();
	const FVector CameraRight = FollowCamera ? FollowCamera->GetRightVector() : GetActorRightVector();

	const FVector PlanarForward = FVector(CameraForward.X, CameraForward.Y, 0.f).GetSafeNormal();
	const FVector PlanarRight = FVector(CameraRight.X, CameraRight.Y, 0.f).GetSafeNormal();

	if (PlanarForward.IsNearlyZero() || PlanarRight.IsNearlyZero())
	{
		return;
	}

	const FVector MoveDirection = PlanarForward * Axis.Y + PlanarRight * Axis.X;
	const float InputScale = FMath::Min(Axis.Length(), 1.0f);
	if (!MoveDirection.IsNearlyZero() && InputScale > 0.0f)
	{
		AddMovementInput(MoveDirection.GetSafeNormal(), InputScale);
	}
}

void APDPlayerCharacter::HandleAttack()
{
	if (CurAIState == EAIState::Die)
	{
		return;
	}

	Attack();
}

void APDPlayerCharacter::HandleInventory()
{
	if (APDBattleGameMode* BattleGM = GetWorld()->GetAuthGameMode<APDBattleGameMode>())
	{
		BattleGM->ToggleBattleInventory();
	}
}

float APDPlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
                                      AController* EventInstigator, AActor* DamageCauser)
{
	float Result = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (APDBattleGameMode* BattleGM = GetWorld()->GetAuthGameMode<APDBattleGameMode>())
	{
		BattleGM->UpdatePlayerHP(UnitInfo.CurHP, UnitInfo.MaxHP);
	}

	return Result;
}

void APDPlayerCharacter::ApplyCameraSettings()
{
	if (CameraBoom)
	{
		CameraBoom->bUsePawnControlRotation = false;
		CameraBoom->bInheritPitch = false;
		CameraBoom->bInheritYaw = false;
		CameraBoom->bInheritRoll = false;
		CameraBoom->SetUsingAbsoluteRotation(true);

		CameraBoom->TargetArmLength = 700.0f;
		CameraBoom->SocketOffset = FVector(0.0f, 0.0f, 200.0f);

		const float BaseYaw = Controller ? Controller->GetControlRotation().Yaw : GetActorRotation().Yaw;
		CameraBoom->SetWorldRotation(FRotator(-20.0f, BaseYaw, 0.0f));
	}

	if (FollowCamera)
	{
		FollowCamera->bUsePawnControlRotation = false;
	}
}

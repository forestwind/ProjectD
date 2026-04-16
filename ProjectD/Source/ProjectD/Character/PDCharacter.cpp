// Fill out your copyright notice in the Description page of Project Settings.


#include "PDCharacter.h"
#include "UI/Battle/PDHpBarWidgetComponent.h"
#include "PDAIController.h"
#include "Engine/GameInstance.h"
#include "Table/PDTableManagerSubsystem.h"
#include "DataAsset/PDUnitDataAsset.h"
#include "DataAsset/Stage/PDStageDataAsset.h"
#include "Table/PDUnitRow.h"
#include "Table/PDUnitStatRow.h"
#include "Table/PDUnitLevelRow.h"
#include "Battle/PDBattleGameMode.h"
#include "UI/Battle/PDUIBattleDamageNumWidget.h"
#include "UI/Core/PDUIManagerSubsystem.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "PDDefine.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"

// Sets default values
APDCharacter::APDCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	//AIControllerClass = APDAIController::StaticClass();
	UnitAsset = nullptr;
	IdleMontage = nullptr;
	AttackMontage = nullptr;
	DieMontage = nullptr;
	VictoryMontage = nullptr;

	UnitGuid = FGuid();
	UnitID = 1;
	CurAIState = EAIState::Ready;

	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_PDPAWN);
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	HpBarWidgetComponent = CreateDefaultSubobject<UPDHpBarWidgetComponent>(TEXT("HpBarWidget"));
	HpBarWidgetComponent->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void APDCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APDCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APDCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APDCharacter::SetInfo(const int32 InUnitTableID, const int32 InLevel, const FGuid& InUnitGuid)
{
	UnitID = InUnitTableID;
	UnitGuid = InUnitGuid;
	UnitInfo.Level = InLevel;
	LoadInfo(UnitID);
}

void APDCharacter::UpdateUIHpBar(float Value)
{
	if (HpBarWidgetComponent)
	{
		HpBarWidgetComponent->UpdateHpBar(Value);
	}
}

void APDCharacter::Attack()
{
	ChangeAIState(EAIState::Attack);
}

void APDCharacter::CheckAttack()
{
	FHitResult OutHitResult;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	const float AttackRange = 50.0f;
	const float AttackRadius = 50.0f;
	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + GetActorForwardVector() * AttackRange;

	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_PDACTION, FCollisionShape::MakeSphere(AttackRadius), Params);
	if (HitDetected)
	{
		FDamageEvent DamageEvent;
		OutHitResult.GetActor()->TakeDamage(UnitInfo.Attack, DamageEvent, GetController(), this);
	}

#if ENABLE_DRAW_DEBUG

	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	float CapsuleHalfHeight = AttackRange * 0.5f;
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 5.0f);

#endif
}

float APDCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	TakeDamageInternal(DamageAmount);

	return DamageAmount;
}

void APDCharacter::LoadInfo(const int32 UnitTableID)
{
	UGameInstance* GI = GetGameInstance();
	if (!GI)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PD] GameInstance is null."));
		return;
	}

	UPDTableManagerSubsystem* TableManager = GI->GetSubsystem<UPDTableManagerSubsystem>();
	if (!TableManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PD] TableManagerSubsystem is null."));
		return;
	}

	const FPDUnitRow* UnitTable = TableManager->GetUnit(UnitTableID);
	if (!UnitTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PD] Failed to load UnitTable : %d"), UnitTableID);
		return;
	}

	UnitAsset = TableManager->GetUnitDataAssetByName(UnitTable->DataAssetName);
	if (!UnitAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PD] Failed to load UnitDataAsset: %s"), *UnitTable->DataAssetName);
		return;
	}

	LoadStat(TableManager, UnitTable);

	SpawnDefaultController();
	LoadAnimation();
	
	UE_LOG(LogTemp, Log, TEXT("[PD] [%d, %s] Unit loaded OK"), UnitTableID, *UnitTable->UnitName);
}

void APDCharacter::LoadStat(const UPDTableManagerSubsystem* InTableManager, const FPDUnitRow* InUnitTable)
{
	if (InTableManager == nullptr || InUnitTable == nullptr)
	{
		return;
	}

	const FPDUnitStatRow* StatData = InTableManager->GetUnitStat(InUnitTable->StatID);
	if (StatData == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PD] Failed to load UnitStatTable : %d"), InUnitTable->StatID);
		return;
	}

	const FPDUnitLevelRow* LevelData = InTableManager->GetUnitLevel(UnitInfo.Level);
	if (LevelData == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PD] Failed to load UnitLevelTable : %d"), UnitInfo.Level);
		return;
	}

	// setting info
	UnitInfo.MaxHP = UnitInfo.CurHP = StatData->HP + LevelData->AddHP;
	UnitInfo.Attack = StatData->Attack + LevelData->AddAttack;
	UnitInfo.Defense = StatData->Defense + LevelData->AddDefence;
	UpdateUIHpBar(GetHpPercent());
}

void APDCharacter::LoadAnimation()
{
	if (UnitAsset == nullptr)
	{
		return;
	}

	IdleMontage = UnitAsset->IdleMontage.LoadSynchronous();
	if (IdleMontage == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("[PD] Invalid IdleMontage [UnitID: %d]"), UnitID);
	}

	AttackMontage = UnitAsset->AttackMontage.LoadSynchronous();
	if (AttackMontage == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("[PD] Invalid AttackMontage [UnitID: %d]"), UnitID);
	}

	DamagedMontage = UnitAsset->DamageMontage.LoadSynchronous();
	if (DamagedMontage == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("[PD] Invalid DamagedMontage [UnitID: %d]"), UnitID);
	}

	DieMontage = UnitAsset->DieMontage.LoadSynchronous();
	if (DieMontage == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("[PD] Invalid DieMontage [UnitID: %d]"), UnitID);
	}

	MoveMontage = UnitAsset->MoveMontage.LoadSynchronous();
	if (MoveMontage == nullptr)
	{
		//UE_LOG(LogTemp, Log, TEXT("[PD] Invalid MoveMontage [UnitID: %d]"), UnitID);
	}

	VictoryMontage = UnitAsset->VictoryMontage.LoadSynchronous();
	if (VictoryMontage == nullptr)
	{
		//UE_LOG(LogTemp, Log, TEXT("[PD] Invalid VictoryMontage [UnitID: %d]"), UnitID);
	}
	ChangeAIState(EAIState::Idle);
}

void APDCharacter::ChangeAnimation(EAIState InAIState)
{
	if (InAIState == EAIState::Max || CurAIState == InAIState)
	{
		return;
	}
	
	CurAIState = InAIState;
	UAnimInstance* AnimInstance = GetMesh() == nullptr ? nullptr : GetMesh()->GetAnimInstance();
	if (AnimInstance == nullptr)
	{
		return;
	}

	bool bBindDelegate = false;
	bool bMove = false;
	UAnimMontage* PlayMontage = nullptr;

	switch (InAIState)
	{
	case EAIState::Idle:
	{
		//PlayMontage = IdleMontage;
		bMove = true;
	}
	break;
	case EAIState::Attack:
	{
		PlayMontage = AttackMontage;
		bBindDelegate = true;
	}
	break;
	case EAIState::Damage:
	{
		PlayMontage = DamagedMontage;
		bBindDelegate = true;
	}
	break;
	case EAIState::Die:
	{
		PlayMontage = DieMontage;
		bBindDelegate = true;
		SetActorEnableCollision(false);
		AnimInstance->StopAllMontages(0.0f);
	}
	break;
	case EAIState::Victory:
	{
		PlayMontage = VictoryMontage;
		bBindDelegate = true;
	}
	break;
	}

	if (bMove == false)
	{
		if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
		{
			MovementComponent->SetMovementMode(EMovementMode::MOVE_Walking);
		}
	}

	if (PlayMontage)
	{
		PlayAnimMontage(PlayMontage);

		if (bBindDelegate)
		{
			FOnMontageBlendingOutStarted BlendOutDelegate;
			BlendOutDelegate.BindUObject(this, &APDCharacter::AnimationEnd);
			AnimInstance->Montage_SetBlendingOutDelegate(BlendOutDelegate, PlayMontage);
		}
	}
}

void APDCharacter::AnimationEnd(UAnimMontage* InMontage, bool bInterrupted)
{
	// 종료/레벨전환 중 인터럽트된 콜백은 무시
	if (bInterrupted)
	{
		return;
	}

	if (InMontage == DieMontage)
	{
		if (APDBattleGameMode* BattleGameMode = Cast<APDBattleGameMode>(GetWorld()->GetAuthGameMode()))
		{
			BattleGameMode->DespawnUnit(UnitGuid);
		}
	}
	else if (InMontage == AttackMontage)
	{
		if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
		{
			MovementComponent->SetMovementMode(EMovementMode::MOVE_Walking);
		}
		NotifyAttackEnd();
		ChangeAIState(EAIState::Idle);
	}
	else if (InMontage == VictoryMontage || InMontage == DamagedMontage)
	{
		ChangeAIState(EAIState::Idle);
	}
}

void APDCharacter::NotifyAttackEnd()
{
}

void APDCharacter::ChangeAIState(EAIState InAIState)
{
	ChangeAnimation(InAIState);
	if (APDAIController* PDAIController = Cast<APDAIController>(GetController()))
	{
		PDAIController->ChangeAIState(InAIState);
	}
}

void APDCharacter::TakeDamageInternal(const float InDamage)
{
	if (UnitInfo.CurHP <= 0)
	{
		return;
	}

	UnitInfo.CurHP = FMath::Max(UnitInfo.CurHP - InDamage, 0.0f);
	UpdateUIHpBar(GetHpPercent());
	ShowUIDamageNum(InDamage);
	UE_LOG(LogTemp, Log, TEXT("[PD][UnitID: %d] CurHP : %d"), UnitID, UnitInfo.CurHP);

	if (UnitInfo.CurHP > 0)
	{
		ChangeAIState(EAIState::Damage);
	}
	else
	{
		ChangeAIState(EAIState::Die);
	}
}

void APDCharacter::ShowUIDamageNum(float InDamage)
{
	APDBattleGameMode* BattleGM = GetWorld()->GetAuthGameMode<APDBattleGameMode>();
	if (!BattleGM)
	{
		return;
	}

	TSubclassOf<UPDUIBattleDamageNumWidget> WidgetClass = BattleGM->GetDamageNumWidgetClass();
	if (!WidgetClass)
	{
		return;
	}

	UGameInstance* GI = GetGameInstance();
	if (!GI)
	{
		return;
	}

	UPDUIManagerSubsystem* UIManager = GI->GetSubsystem<UPDUIManagerSubsystem>();
	if (!UIManager)
	{
		return;
	}

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC)
	{
		return;
	}

	// 캐릭터 머리 위 월드 좌표 → UMG Canvas 좌표 변환 (DPI 자동 보정)
	const FVector HeadLocation = GetActorLocation() + FVector(0.f, 0.f, 80.f);
	FVector2D WidgetPos;
	if (!UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(PC, HeadLocation, WidgetPos, false))
	{
		return;
	}

	UPDUIBattleDamageNumWidget* Widget = CreateWidget<UPDUIBattleDamageNumWidget>(GI, WidgetClass);
	if (!Widget)
	{
		return;
	}
	
	UIManager->AddWidgetToPanelOverlayAtPosition(Widget, WidgetPos, FVector2D(0.5f, 1.0f));
	Widget->ShowDamage(InDamage);
}

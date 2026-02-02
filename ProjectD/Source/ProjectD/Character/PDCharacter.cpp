// Fill out your copyright notice in the Description page of Project Settings.


#include "PDCharacter.h"
#include "PDAIController.h"
#include "Engine/GameInstance.h"
#include "../Table/PDTableManagerSubsystem.h"
#include "../DataAsset/PDUnitDataAsset.h"
#include "../DataAsset/Stage/PDStageDataAsset.h"
#include "../Table/PDUnitRow.h"
#include "../Table/PDUnitStatRow.h"
#include "../Table/PDUnitLevelRow.h"

// Sets default values
APDCharacter::APDCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AIControllerClass = APDAIController::StaticClass();
	UnitAsset = nullptr;
	IdleMontage = nullptr;
	AttackMontage = nullptr;
	DieMontage = nullptr;
	VictoryMontage = nullptr;

	UnitGuid = FGuid();
	UnitID = 1;
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

void APDCharacter::SetInfo(const int32 InUnitTableID, const FGuid& InUnitGuid)
{
	UnitID = InUnitTableID;
	UnitGuid = InUnitGuid;
	LoadInfo(UnitID);
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

	DieMontage = UnitAsset->DieMontage.LoadSynchronous();
	if (DieMontage == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("[PD] Invalid DieMontage [UnitID: %d]"), UnitID);
	}

	VictoryMontage = UnitAsset->VictoryMontage.LoadSynchronous();
	if (VictoryMontage == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("[PD] Invalid VictoryMontage [UnitID: %d]"), UnitID);
	}
}

void APDCharacter::ChangeAnimation(EAIState InAIState)
{
	if (InAIState == EAIState::Max)
	{
		return;
	}

	UAnimInstance* AnimInstance = GetMesh() == nullptr ? nullptr : GetMesh()->GetAnimInstance();
	if (AnimInstance == nullptr)
	{
		return;
	}

	if (InAIState == EAIState::Idle)
	{
		PlayAnimMontage(IdleMontage);
	}
	else if (InAIState == EAIState::Attack)
	{
		PlayAnimMontage(AttackMontage);
		FOnMontageBlendingOutStarted BlendOutDelegate;
		BlendOutDelegate.BindUObject(this, &APDCharacter::AnimationEnd);
		AnimInstance->Montage_SetBlendingOutDelegate(BlendOutDelegate, AttackMontage);
	}
	else if (InAIState == EAIState::Die)
	{
		PlayAnimMontage(DieMontage);
	}
	else if (InAIState == EAIState::Victory)
	{
		PlayAnimMontage(VictoryMontage);
		FOnMontageBlendingOutStarted BlendOutDelegate;
		BlendOutDelegate.BindUObject(this, &APDCharacter::AnimationEnd);
		AnimInstance->Montage_SetBlendingOutDelegate(BlendOutDelegate, VictoryMontage);
	}
}

void APDCharacter::AnimationEnd(UAnimMontage* InMontage, bool bInterrupted)
{
	if (InMontage == VictoryMontage || InMontage == AttackMontage)
	{
		ChangeAIState(EAIState::Idle);
	}
}

void APDCharacter::ChangeAIState(EAIState InAIState)
{
	if (APDAIController* PDAIController = Cast<APDAIController>(GetController()))
	{
		PDAIController->ChangeAIState(InAIState);
	}
}

void APDCharacter::Attack()
{
}

void APDCharacter::TakeDamaged(const float InDamage)
{
	UnitInfo.CurHP = FMath::Max(UnitInfo.CurHP - InDamage, 0.0f);
	UE_LOG(LogTemp, Log, TEXT("[PD][UnitID: %d] CurHP : %d"), UnitID, UnitInfo.CurHP);
	if (UnitInfo.CurHP > 0)
	{
		//ChangeAIState(EAIState::Damage);
	}
	else
	{
		ChangeAIState(EAIState::Die);
	}
}

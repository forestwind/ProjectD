// Fill out your copyright notice in the Description page of Project Settings.


#include "PDCharacter.h"
#include "PDAIController.h"
#include "Engine/GameInstance.h"
#include "../Table/PDTableManagerSubsystem.h"
#include "../DataAsset/PDUnitDataAsset.h"
#include "../DataAsset/Stage/PDStageDataAsset.h"
#include "../Table/PDUnitRow.h"

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
	

	LoadInfo(UnitID);
	TestLogStageDataAsset_Stage(TEXT("DA_ForestMap"));
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

void APDCharacter::LoadInfo(const int32 UnitTableID)
{
	UGameInstance* GI = GetGameInstance();
	if (!GI)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PD][Test] GameInstance is null."));
		return;
	}

	UPDTableManagerSubsystem* TableManager = GI->GetSubsystem<UPDTableManagerSubsystem>();
	if (!TableManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PD][Test] TableManagerSubsystem is null."));
		return;
	}

	const FPDUnitRow* UnitTable = TableManager->GetUnit(UnitTableID);
	if (!UnitTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PD][Test] Failed to load UnitTable : %d"), UnitTableID);
		return;
	}

	UnitAsset = TableManager->GetUnitDataAssetByName(UnitTable->DataAssetName);
	if (!UnitAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PD][Test] Failed to load UnitDataAsset: %s"), *UnitTable->DataAssetName);
		return;
	}

	const FString UnitBPPath = UnitAsset->UnitBP.ToSoftObjectPath().ToString();
	const FString IconPath = UnitAsset->Icon.ToSoftObjectPath().ToString();

	UE_LOG(LogTemp, Log, TEXT("[PD][Test] %d Unit loaded OK"), UnitTableID);
	UE_LOG(LogTemp, Log, TEXT("[PD][Test] - UnitName: %s"), *UnitTable->UnitName);
	UE_LOG(LogTemp, Log, TEXT("[PD][Test] - UnitBP: %s"), UnitBPPath.IsEmpty() ? TEXT("(None)") : *UnitBPPath);

	SpawnDefaultController();
	LoadAnimation();
}

void APDCharacter::TestLogStageDataAsset_Stage(const FString& StageAssetName)
{
	UGameInstance* GI = GetGameInstance();
	if (!GI)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PD][Test] GameInstance is null."));
		return;
	}

	UPDTableManagerSubsystem* TableManager = GI->GetSubsystem<UPDTableManagerSubsystem>();
	if (!TableManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PD][Test] TableManagerSubsystem is null."));
		return;
	}

	const UPDStageDataAsset* StageTable = TableManager->GetStageDataAssetByName(StageAssetName);
	if (!StageTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PD][Test] Failed to load StageTable : %s"), *StageAssetName);
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("[PD][Test][Stage] Loaded OK. Name: %s Path: %s"), *StageAssetName, *StageTable->GetPathName());
	UE_LOG(LogTemp, Log, TEXT("[PD][Test][Stage] Rounds: %d"), StageTable->Rounds.Num());

	for (int32 RoundIdx = 0; RoundIdx < StageTable->Rounds.Num(); ++RoundIdx)
	{
		const FStageRoundSpawnData& Round = StageTable->Rounds[RoundIdx];
		UE_LOG(LogTemp, Log, TEXT("[PD][Test][Stage][%s] Round[%d] Ally:%d Enemy:%d"),
			*StageAssetName,
			RoundIdx,
			Round.AllySpawnPoints.Num(),
			Round.EnemySpawnPoints.Num());

		auto LogSpawnPoints = [&](const TCHAR* SideLabel, const TArray<FStageUnitSpawnPoint>& Points)
		{
			for (int32 PointIdx = 0; PointIdx < Points.Num(); ++PointIdx)
			{
				const FStageUnitSpawnPoint& P = Points[PointIdx];
				UE_LOG(LogTemp, Log, TEXT("[PD][Test][Stage][%s]  - Round[%d] %s[%d] Slot:%d Loc:%s Rot:%s"),
					*StageAssetName,
					RoundIdx,
					SideLabel,
					PointIdx,
					P.SlotIndex,
					*P.Location.ToString(),
					*P.Rotation.ToString());
			}
		};

		LogSpawnPoints(TEXT("Ally"), Round.AllySpawnPoints);
		LogSpawnPoints(TEXT("Enemy"), Round.EnemySpawnPoints);
	}
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
		UE_LOG(LogTemp, Log, TEXT("Invalid IdleMontage [UnitID: %d]"), UnitID);
	}

	AttackMontage = UnitAsset->AttackMontage.LoadSynchronous();
	if (AttackMontage == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Invalid AttackMontage [UnitID: %d]"), UnitID);
	}

	DieMontage = UnitAsset->DieMontage.LoadSynchronous();
	if (DieMontage == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Invalid DieMontage [UnitID: %d]"), UnitID);
	}

	VictoryMontage = UnitAsset->VictoryMontage.LoadSynchronous();
	if (VictoryMontage == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Invalid VictoryMontage [UnitID: %d]"), UnitID);
	}

	// test
	ChangeAnimation(EAIState::Attack);
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
	APDAIController* PDAIController = Cast<APDAIController>(GetController());
	if (PDAIController == nullptr)
	{
		return;
	}

	if (InMontage == VictoryMontage || InMontage == AttackMontage)
	{
		PDAIController->ChangeAIState(EAIState::Idle);
	}
}

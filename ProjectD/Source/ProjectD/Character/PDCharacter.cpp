// Fill out your copyright notice in the Description page of Project Settings.


#include "PDCharacter.h"
#include "PDAIController.h"
#include "Engine/GameInstance.h"
#include "../Table/PDTableManagerSubsystem.h"
#include "../DataAsset/PDUnitDataAsset.h"
#include "../Table/PDUnitRow.h"

// Sets default values
APDCharacter::APDCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AIControllerClass = APDAIController::StaticClass();
	UnitGuid = FGuid();
}

// Called when the game starts or when spawned
void APDCharacter::BeginPlay()
{
	Super::BeginPlay();
	

	TestLogUnitDataAsset_Unit(1);
	TestLogUnitDataAsset_Unit(2);
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

void APDCharacter::TestLogUnitDataAsset_Unit(int32 UnitTableID)
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

	UPDUnitDataAsset* DA = TableManager->GetUnitDataAssetByName(UnitTable->DataAssetName);
	if (!DA)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PD][Test] Failed to load UnitDataAsset: %s"), *UnitTable->DataAssetName);
		return;
	}

	const FString UnitBPPath = DA->UnitBP.ToSoftObjectPath().ToString();
	const FString IconPath = DA->Icon.ToSoftObjectPath().ToString();

	UE_LOG(LogTemp, Log, TEXT("[PD][Test] %d Unit loaded OK"), UnitTableID);
	UE_LOG(LogTemp, Log, TEXT("[PD][Test] - UnitName: %s"), *UnitTable->UnitName);
	UE_LOG(LogTemp, Log, TEXT("[PD][Test] - UnitBP: %s"), UnitBPPath.IsEmpty() ? TEXT("(None)") : *UnitBPPath);
}


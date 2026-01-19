// Fill out your copyright notice in the Description page of Project Settings.


#include "PDCharacter.h"
#include "PDAIController.h"
#include "Engine/GameInstance.h"
#include "../Table/PDTableManagerSubsystem.h"
#include "../DataAsset/PDUnitDataAsset.h"

// Sets default values
APDCharacter::APDCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AIControllerClass = APDAIController::StaticClass();
}

// Called when the game starts or when spawned
void APDCharacter::BeginPlay()
{
	Super::BeginPlay();
	

	TestLogUnitDataAsset_DA_Unit_001();
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

void APDCharacter::TestLogUnitDataAsset_DA_Unit_001()
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

	UPDUnitDataAsset* DA = TableManager->GetUnitDataAssetByName(TEXT("DA_Unit_001"));
	if (!DA)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PD][Test] Failed to load UnitDataAsset: DA_Unit_001"));
		return;
	}

	const FString UnitBPPath = DA->UnitBP.ToSoftObjectPath().ToString();
	const FString MeshPath = DA->Mesh.ToSoftObjectPath().ToString();
	const FString IconPath = DA->Icon.ToSoftObjectPath().ToString();

	UE_LOG(LogTemp, Log, TEXT("[PD][Test] DA_Unit_001 loaded OK"));
	UE_LOG(LogTemp, Log, TEXT("[PD][Test] - UnitBP: %s"), UnitBPPath.IsEmpty() ? TEXT("(None)") : *UnitBPPath);
	UE_LOG(LogTemp, Log, TEXT("[PD][Test] - Mesh  : %s"), MeshPath.IsEmpty() ? TEXT("(None)") : *MeshPath);
	UE_LOG(LogTemp, Log, TEXT("[PD][Test] - Icon  : %s"), IconPath.IsEmpty() ? TEXT("(None)") : *IconPath);
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "ModelManager.h"

#include "../Character/PDCharacter.h"
#include "../Table/PDTableManagerSubsystem.h"
#include "../Table/PDUnitRow.h"
#include "Kismet/GameplayStatics.h"


UModelManager::UModelManager()
{
	CharacterMap.Empty();
}

void UModelManager::BeginDestroy()
{
	CharacterMap.Empty();

	Super::BeginDestroy();
}

APDCharacter* UModelManager::SpawnCharacter(const int32 InUnitID, const FVector& InPosition, const FRotator& InRotation)
{
	UPDTableManagerSubsystem* TableManager = UGameInstance::GetSubsystem<UPDTableManagerSubsystem>(UGameplayStatics::GetGameInstance(this));
	if (TableManager == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid TableManager"));
		return nullptr;
	}

	const FPDUnitRow* UnitData = TableManager->GetUnit(InUnitID);
	if (UnitData == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid UnitData %d"), InUnitID);
		return nullptr;
	}

	FString Path = TEXT("/Game/Unit/Blueprints/BP_Nana.BP_Nana_C");
	UClass* ObjectClass = StaticLoadClass(UObject::StaticClass(), NULL, *Path, NULL, LOAD_None, NULL);
	if (ObjectClass == nullptr)
	{
		return nullptr;
	}

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	APDCharacter* PDCharacter = GetWorld()->SpawnActor<APDCharacter>(ObjectClass, InPosition, InRotation, SpawnInfo);
	if (PDCharacter)
	{
		FGuid NewGuid = FGuid::NewGuid();
		// init character
		CharacterMap.Add(NewGuid, PDCharacter);
	}

	return PDCharacter;
}

void UModelManager::DespawnCharacter(const FGuid InUnitGuid, bool InImmediately)
{
	if (CharacterMap.Contains(InUnitGuid))
	{
		if(APDCharacter* PDCharacter = *CharacterMap.Find(InUnitGuid))
		{
			if (InImmediately)
			{
				PDCharacter->Destroy();
			}
			else
			{
				// death character

			}
		}
		CharacterMap.Remove(InUnitGuid);
	}
}

APDCharacter* UModelManager::FindCharacter(const FGuid InUnitGuid)
{
	APDCharacter* PDCharacter = nullptr;
	if (CharacterMap.Contains(InUnitGuid))
	{
		PDCharacter = *CharacterMap.Find(InUnitGuid);
	}

	return PDCharacter;
}
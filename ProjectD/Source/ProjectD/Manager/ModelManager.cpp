// Fill out your copyright notice in the Description page of Project Settings.


#include "ModelManager.h"

#include "Battle/PDBattleItemActor.h"
#include "Character/PDCharacter.h"
#include "Table/PDTableManagerSubsystem.h"
#include "Table/PDUnitRow.h"
#include "DataAsset/PDUnitDataAsset.h"
#include "Kismet/GameplayStatics.h"


UModelManager::UModelManager()
{
	CharacterMap.Empty();
	BattleItemMap.Empty();
}

void UModelManager::BeginDestroy()
{
	for (TPair<FGuid, TObjectPtr<APDBattleItemActor>>& Pair : BattleItemMap)
	{
		if (APDBattleItemActor* PDBattleItemActor = Pair.Value.Get())
		{
			PDBattleItemActor->Destroy();
		}
	}
	BattleItemMap.Empty();
	CharacterMap.Empty();

	Super::BeginDestroy();
}

APDCharacter* UModelManager::SpawnCharacter(const int32 InUnitID, const int32 InLevel, const FVector& InPosition, const FRotator& InRotation)
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

	UPDUnitDataAsset* UnitAsset = TableManager->GetUnitDataAssetByName(UnitData->DataAssetName);
	if (!UnitAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PD][Test] Failed to load UnitDataAsset: %s"), *UnitData->DataAssetName);
		return nullptr;
	}

	const FString UnitBPPath = UnitAsset->UnitBP.ToSoftObjectPath().ToString();
	UClass* ObjectClass = StaticLoadClass(UObject::StaticClass(), NULL, *UnitBPPath, NULL, LOAD_None, NULL);
	if (ObjectClass == nullptr)
	{
		return nullptr;
	}

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	APDCharacter* PDCharacter = GetWorld()->SpawnActor<APDCharacter>(ObjectClass, InPosition, InRotation, SpawnInfo);
	AddCharacter(PDCharacter, InUnitID, InLevel);

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
				PDCharacter->Destroy();
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

void UModelManager::AddCharacter(APDCharacter* PDCharacter, const int32 InUnitID, const int32 InLevel)
{
	if (PDCharacter == nullptr)
	{
		return;
	}

	FGuid NewGuid = FGuid::NewGuid();
	PDCharacter->SetInfo(InUnitID, InLevel, NewGuid);
	CharacterMap.Add(NewGuid, PDCharacter);
}

APDBattleItemActor* UModelManager::SpawnBattleItem(int32 ItemID, const FVector& InPosition, const FRotator& InRotation)
{
	UPDTableManagerSubsystem* TableManager = UGameInstance::GetSubsystem<UPDTableManagerSubsystem>(UGameplayStatics::GetGameInstance(this));
	if (TableManager == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid TableManager"));
		return nullptr;
	}

	if (TableManager->GetBattleItem(ItemID) == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid ItemID %d"), ItemID);
		return nullptr;
	}

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	APDBattleItemActor* PDBattleItemActor = GetWorld()->SpawnActor<APDBattleItemActor>(APDBattleItemActor::StaticClass(), InPosition, InRotation, SpawnInfo);
	if (PDBattleItemActor == nullptr)
	{
		return nullptr;
	}

	FGuid NewGuid = FGuid::NewGuid();
	PDBattleItemActor->Initialize(NewGuid, ItemID);
	BattleItemMap.Add(NewGuid, PDBattleItemActor);

	return PDBattleItemActor;
}

void UModelManager::DespawnBattleItem(const FGuid InItemGuid)
{
	if (BattleItemMap.Contains(InItemGuid))
	{
		if (APDBattleItemActor* PDBattleItemActor = *BattleItemMap.Find(InItemGuid))
		{
			PDBattleItemActor->Destroy();
		}
		BattleItemMap.Remove(InItemGuid);
	}
}

APDBattleItemActor* UModelManager::FindBattleItem(const FGuid InItemGuid)
{
	APDBattleItemActor* PDBattleItemActor = nullptr;
	if (BattleItemMap.Contains(InItemGuid))
	{
		PDBattleItemActor = *BattleItemMap.Find(InItemGuid);
	}

	return PDBattleItemActor;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "PDBattleGameMode.h"
#include "../Character/PDCharacter.h"

void APDBattleGameMode::BeginPlay()
{
	Super::BeginPlay();

	FString Path = TEXT("/Game/Unit/Blueprints/BP_Nana.BP_Nana_C");
	UClass* ObjectClass = StaticLoadClass(UObject::StaticClass(), NULL, *Path, NULL, LOAD_None, NULL);
	if (ObjectClass == nullptr)
	{
		return;
	}

	FVector Pos(4979.f, 5132.f, -193.f);
	FRotator Rot(0.0f, 20.0f, 0.0f);
	FActorSpawnParameters blueSpawnInfo;
	blueSpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	APDCharacter* PDCharacter = GetWorld()->SpawnActor<APDCharacter>(ObjectClass, Pos, Rot, blueSpawnInfo);
}
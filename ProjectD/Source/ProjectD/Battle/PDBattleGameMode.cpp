// Fill out your copyright notice in the Description page of Project Settings.


#include "PDBattleGameMode.h"

#include "../Manager/ModelManager.h"
#include "../Character/PDCharacter.h"


void APDBattleGameMode::BeginPlay()
{
	Super::BeginPlay();

	FVector Pos(4979.f, 5132.f, -193.f);
	FRotator Rot(0.0f, 20.0f, 0.0f);
	if (ModelManager)
	{
		ModelManager->SpawnCharacter(1, Pos, Rot);
	}
}
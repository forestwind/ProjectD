// Fill out your copyright notice in the Description page of Project Settings.


#include "PDGameModeBase.h"

#include "Manager/ModelManager.h"


APDGameModeBase::APDGameModeBase()
{
	ModelManager = nullptr;
}

void APDGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	ModelManager = NewObject<UModelManager>(this);
}

void APDGameModeBase::BeginDestroy()
{
	ModelManager = nullptr;

	Super::BeginDestroy();
}

void APDGameModeBase::AddCharacter(APDCharacter* PDCharacter, const int32 InUnitID)
{
	if (ModelManager)
	{
		ModelManager->AddCharacter(PDCharacter, InUnitID);
	}
}
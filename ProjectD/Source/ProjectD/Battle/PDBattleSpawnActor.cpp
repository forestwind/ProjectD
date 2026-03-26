// Fill out your copyright notice in the Description page of Project Settings.

#include "Battle/PDBattleSpawnActor.h"
#include "Components/SceneComponent.h"
#include "Components/ArrowComponent.h"

// Sets default values
APDBattleSpawnActor::APDBattleSpawnActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ArrowComponent->SetupAttachment(SceneRoot);
}
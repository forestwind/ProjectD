// Fill out your copyright notice in the Description page of Project Settings.

#include "Battle/PDBattleItemActor.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Character/PDPlayerCharacter.h"
#include "Battle/PDBattleGameMode.h"
#include "PDDefine.h"

APDBattleItemActor::APDBattleItemActor()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent);

	BoxComponent->SetCollisionProfileName(CPROFILE_PDTRIGGER);
	BoxComponent->SetBoxExtent(FVector(40.f, 40.f, 40.f));
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &APDBattleItemActor::OnOverlapBegin);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCollisionProfileName(TEXT("NoCollision"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeMeshAsset.Succeeded())
	{
		MeshComponent->SetStaticMesh(CubeMeshAsset.Object);
		MeshComponent->SetRelativeScale3D(FVector(0.35f));
	}
}

void APDBattleItemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorLocalRotation(FRotator(0.f, RotationSpeed * DeltaTime, 0.f));
}

void APDBattleItemActor::Initialize(const FGuid& InItemGuid, int32 InItemID)
{
	ItemGuid = InItemGuid;
	ItemID = InItemID;
}

void APDBattleItemActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 아이템과 충돌한 액터가 플레이어 캐릭터인 경우에만 처리
	if (OtherActor && OtherComp)
	{
		APDPlayerCharacter* PlayerCharacter = Cast<APDPlayerCharacter>(OtherActor);
		if (PlayerCharacter)
		{
			// 아이템 획득 처리
			UE_LOG(LogTemp, Log, TEXT("[PD][BattleItemActor][OnOverlapBegin] Player picked up item with ID: %d"), ItemID);
			// 아이템 액터 제거
			if (APDBattleGameMode* BattleGameMode = Cast<APDBattleGameMode>(GetWorld()->GetAuthGameMode()))
			{
				BattleGameMode->DespawnItem(ItemGuid);
			}
		}
	}
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PDBattleItemActor.generated.h"

class UStaticMeshComponent;
class UBoxComponent;

/**
 * 레벨에 배치되는 배틀 아이템 액터
 */
UCLASS()
class PROJECTD_API APDBattleItemActor : public AActor
{
	GENERATED_BODY()

public:
	APDBattleItemActor();

	void Initialize(const FGuid& InItemGuid, int32 InItemID);

	FGuid GetItemGuid() const { return ItemGuid; }
	int32 GetItemID() const { return ItemID; }

	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "BattleItemActor")
	float RotationSpeed = 90.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BattleItemActor")
	TObjectPtr<UStaticMeshComponent> MeshComponent;

protected:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BattleItemActor")
	TObjectPtr<UBoxComponent> BoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BattleItemActor")
	FGuid ItemGuid;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BattleItemActor")
	int32 ItemID = 0;
};

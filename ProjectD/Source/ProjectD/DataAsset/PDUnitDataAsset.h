// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/DataAsset.h"
#include "PDUnitDataAsset.generated.h"

class UTexture2D;
class USkeletalMesh;
class AActor;

/**
 * 유닛 데이터 에셋
 * UnitDataTable에 해당 경로로 매칭
 * - 언리얼 에셋: BP/메시/아이콘 등을 DataAsset에 연결
 */
UCLASS(BlueprintType)
class PROJECTD_API UPDUnitDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	// 유닛 블루프린트 클래스
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Unit|Assets")
	TSoftClassPtr<ACharacter> UnitBP;

	// 유닛 아이콘
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Unit|Assets")
	TSoftObjectPtr<UTexture2D> Icon;
};


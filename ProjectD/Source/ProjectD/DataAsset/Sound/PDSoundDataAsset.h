// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PDSoundDataAsset.generated.h"

class USoundBase;

/**
 * 글로벌 BGM 레퍼런스를 보관하는 데이터 에셋
 * PDGameInstance에 할당하여 사용
 */
UCLASS(BlueprintType)
class PROJECTD_API UPDSoundDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	/** 타이틀 화면 BGM */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound|BGM")
	TSoftObjectPtr<USoundBase> BGM_Title;

	/** 전투 맵 BGM */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound|BGM")
	TSoftObjectPtr<USoundBase> BGM_Battle;
};

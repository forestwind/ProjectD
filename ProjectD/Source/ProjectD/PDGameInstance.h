// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "UObject/SoftObjectPtr.h"
#include "PDGameInstance.generated.h"

class UPDUIRootWidget;
class UPDSoundDataAsset;

/**
 *
 */
UCLASS()
class PROJECTD_API UPDGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSoftClassPtr<UPDUIRootWidget> RootUIClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sound")
	TSoftObjectPtr<UPDSoundDataAsset> SoundDataAsset;

	/** 이미지 텍스처가 위치하는 베이스 경로 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Image")
	FString ImageBasePath = TEXT("/Game/External/Image");
};

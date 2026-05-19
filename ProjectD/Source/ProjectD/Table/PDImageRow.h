// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PDImageRow.generated.h"

USTRUCT(BlueprintType)
struct PROJECTD_API FPDImageRow : public FTableRowBase
{
	GENERATED_BODY()

	/** 다른 테이블에서 이미지를 참조할 때 사용하는 식별자 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Image")
	FName ImageName;

	/** GameInstance.ImageBasePath 아래에 있는 실제 텍스처 파일명 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Image")
	FName FileName;
};
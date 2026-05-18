// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PDDefine.h"
#include "PDUIBaseRow.generated.h"

// CSV(DataTable)로 관리하는 UI 데이터 (Content/Table/Raw/CSV/UITable.csv)
// WidgetClassPath 예시: /Game/UI/Battle/WBP_BattleMain.WBP_BattleMain_C
USTRUCT(BlueprintType)
struct PROJECTD_API FPDUIBase : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	EUIType UIType = EUIType::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	EUILayer UILayer = EUILayer::Screen;

	// 위젯 블루프린트 클래스 풀 경로 (/Game/.../WBP_Foo.WBP_Foo_C)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	FString WidgetClassPath;
};

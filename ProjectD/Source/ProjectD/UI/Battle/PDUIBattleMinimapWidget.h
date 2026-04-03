// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PDUIBattleMinimapWidget.generated.h"

/**
 * 전투 중 플레이어 중심 미니맵 위젯.
 */
UCLASS()
class PROJECTD_API UPDUIBattleMinimapWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	// 미니맵이 커버하는 월드 반경
	UPROPERTY(EditAnywhere, Category = "Minimap")
	float MapRange = 2000.f;

	// 점 한 개의 픽셀 크기 (정사각형 기준).
	UPROPERTY(EditAnywhere, Category = "Minimap")
	float DotSize = 6.f;

	// 플레이어 점 색상
	UPROPERTY(EditAnywhere, Category = "Minimap")
	FLinearColor PlayerColor = FLinearColor::Green;

	// 몬스터 점 색상
	UPROPERTY(EditAnywhere, Category = "Minimap")
	FLinearColor EnemyColor = FLinearColor::Red;

	// Slate 렌더링 패스에서 매 프레임 호출
	virtual int32 NativePaint(
		const FPaintArgs& Args,
		const FGeometry& AllottedGeometry,
		const FSlateRect& MyCullingRect,
		FSlateWindowElementList& OutDrawElements,
		int32 LayerId,
		const FWidgetStyle& InWidgetStyle,
		bool bParentEnabled) const override;

private:
	// 점 하나를 그리는 헬퍼
	void DrawDot(
		const FGeometry& AllottedGeometry,
		FSlateWindowElementList& OutDrawElements,
		int32 LayerId,
		const FVector2D& LocalPos,
		const FLinearColor& Color) const;
};
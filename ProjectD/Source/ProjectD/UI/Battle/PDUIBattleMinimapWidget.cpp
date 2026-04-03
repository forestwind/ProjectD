// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Battle/PDUIBattleMinimapWidget.h"

#include "Battle/PDBattleGameMode.h"
#include "Character/PDMonsterCharacter.h"
#include "Manager/ModelManager.h"
#include "Rendering/DrawElements.h"

int32 UPDUIBattleMinimapWidget::NativePaint(
	const FPaintArgs& Args,
	const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements,
	int32 LayerId,
	const FWidgetStyle& InWidgetStyle,
	bool bParentEnabled) const
{
	LayerId = Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

	// 위젯 픽셀 크기와 중심점 계산 (플레이어는 항상 중심에 표시)
	const FVector2D Size   = AllottedGeometry.GetLocalSize();
	const FVector2D Center = Size * 0.5f;

	UWorld* World = GetWorld();
	if (!World) return LayerId;

	APlayerController* PC = World->GetFirstPlayerController();
	if (!PC) return LayerId;

	APawn* PlayerPawn = PC->GetPawn();
	if (!PlayerPawn) return LayerId;

	const FVector PlayerPos = PlayerPawn->GetActorLocation();

	// 카메라가 바라보는 방향
	const float YawDeg = PC->GetControlRotation().Yaw;

	// 플레이어는 항상 미니맵 중심에 표시
	DrawDot(AllottedGeometry, OutDrawElements, LayerId, Center, PlayerColor);

	APDBattleGameMode* GM = World->GetAuthGameMode<APDBattleGameMode>();
	if (!GM) return LayerId;

	UModelManager* ModelManager = GM->GetModelManager();
	if (!ModelManager) return LayerId;

	for (const auto& Pair : ModelManager->GetCharacterMap())
	{
		APDCharacter* Char = Pair.Value.Get();
		if (!Char) continue;

		// 몬스터만 점으로 표시. 플레이어는 이미 중심에 그렸으므로 스킵.
		if (!Cast<APDMonsterCharacter>(Char)) continue;

		const FVector MonsterPos = Char->GetActorLocation();
		
		const FVector2D ScreenOffset(
			 (MonsterPos.Y - PlayerPos.Y),  // 월드 Y(오른쪽) → 화면 X(오른쪽)
			-(MonsterPos.X - PlayerPos.X)   // 월드 X(앞)     → 화면 -Y(위쪽)
		);

		// 카메라 Yaw만큼 역회전해 카메라 전방이 항상 미니맵 12시를 향하도록 한다.
		const FVector2D Offset = ScreenOffset.GetRotated(-YawDeg);

		// MapRange 기준으로 -1 ~ +1 정규화. 범위 밖 몬스터는 그리지 않는다.
		const FVector2D Ratio = Offset / MapRange;
		if (FMath::Abs(Ratio.X) > 1.f || FMath::Abs(Ratio.Y) > 1.f) continue;

		// 정규화 비율 × 미니맵 반경을 중심에 더해 최종 픽셀 좌표 계산
		const FVector2D DotPos = Center + (Ratio * (Size * 0.5f));

		DrawDot(AllottedGeometry, OutDrawElements, LayerId, DotPos, EnemyColor);
	}

	return LayerId;
}

void UPDUIBattleMinimapWidget::DrawDot(
	const FGeometry& AllottedGeometry,
	FSlateWindowElementList& OutDrawElements,
	int32 LayerId,
	const FVector2D& LocalPos,
	const FLinearColor& Color) const
{
	// 점의 중심이 LocalPos가 되도록 좌상단 좌표를 계산
	const FVector2D TopLeft = LocalPos - FVector2D(DotSize * 0.5f, DotSize * 0.5f);

	// 기본 FSlateBrush는 텍스처 없는 단색 박스. 최종 색상은 Color 인자로 결정된다.
	FSlateBrush Brush;

	FSlateDrawElement::MakeBox(
		OutDrawElements,
		LayerId,
		AllottedGeometry.ToPaintGeometry(
			FVector2D(DotSize, DotSize),
			FSlateLayoutTransform(TopLeft)
		),
		&Brush,
		ESlateDrawEffect::None,
		Color
	);
}

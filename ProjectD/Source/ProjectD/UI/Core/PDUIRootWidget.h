// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PDUIRootWidget.generated.h"

class UCanvasPanel;

/**
 * 게임 전체 UI의 Root
 * - Panel_2D / Panel_Overlay 보유
 * - AddToViewport는 외부(UIManager)에서 1회
 */
UCLASS()
class PROJECTD_API UPDUIRootWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/** 2D 일반 UI 패널 */
	UCanvasPanel* GetPanel2D() const { return Panel2D; }

	/** 항상 위에 오는 Overlay 패널 */
	UCanvasPanel* GetPanelOverlay() const { return PanelOverlay; }

protected:
	/** 일반 UI용 패널 */
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* Panel2D;

	/** 시스템 / 최상단 UI용 패널 */
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* PanelOverlay;
};
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
	UCanvasPanel* GetPanelWorld()  const { return Panel_World; }
	UCanvasPanel* GetPanelScreen() const { return Panel_Screen; }
	UCanvasPanel* GetPanelWindow() const { return Panel_Window; }
	UCanvasPanel* GetPanelPopup()  const { return Panel_Popup; }
	UCanvasPanel* GetPanelSystem() const { return Panel_System; }

protected:
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* Panel_World;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* Panel_Screen;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* Panel_Window;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* Panel_Popup;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* Panel_System;
};
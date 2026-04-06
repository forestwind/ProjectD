// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PDUIManagerSubsystem.generated.h"

class UUserWidget;
class UPDUIRootWidget;
/**
 * 
 */
UCLASS()
class PROJECTD_API UPDUIManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

public:
	UPDUIRootWidget* GetRootUI() const { return RootUI; }

	/** 일반 UI 추가 (panel2d) */
	void AddWidgetToPanel2D(UUserWidget* Widget);

	/** 최상단 UI 추가 (paneloverlay) */
	void AddWidgetToPanelOverlay(UUserWidget* Widget);

	void RemoveWidgetFromPanel2D(UUserWidget* Widget);
	void RemoveWidgetFromPanelOverlay(UUserWidget* Widget);

	void ClearPanel2D();
	void ClearPanelOverlay();

protected:
	/** 생성된 Root UI 인스턴스 */
	UPROPERTY()
	UPDUIRootWidget* RootUI = nullptr;

private:
	void CreateRootUI(TSubclassOf<UPDUIRootWidget> RootWidgetClass);
	/** 뷰포트 준비 시점까지 미룬 AddToViewport (첫 사용 시 1회 호출) */
	void EnsureRootUIAddedToViewport();

private:
	// RootUI를 Viewport에 붙였는지
	bool bRootUIAddedToViewport = false;

	void HandlePostLoadMap(UWorld* LoadedWorld);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UObject/SoftObjectPtr.h"
#include "Engine/StreamableManager.h"
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

	void ClearPanel2D();
	void ClearPanelOverlay();

protected:
	/** 생성된 Root UI 인스턴스 */
	UPROPERTY()
	UPDUIRootWidget* RootUI = nullptr;

private:
	void CreateRootUI(TSubclassOf<UPDUIRootWidget> RootWidgetClass);
	void OnRootUIClassLoaded();

private:
	TSoftClassPtr<UPDUIRootWidget> CachedRootUIClass;
	bool bRootUILoadRequested = false;

	/** Async Load 취소/관리용 */
	TSharedPtr<FStreamableHandle> RootUIStreamableHandle;
};

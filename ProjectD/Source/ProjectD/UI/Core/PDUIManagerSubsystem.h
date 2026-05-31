// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PDDefine.h"
#include "PDUIManagerSubsystem.generated.h"

class UUserWidget;
class UCanvasPanel;
class UPDUIRootWidget;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnWidgetRemoved, EUIType);

UCLASS()
class PROJECTD_API UPDUIManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

public:
	UPDUIRootWidget* GetRootUI() const { return RootUI; }

	/** UIType으로 위젯을 생성해 해당 레이어(전체화면)에 추가하고 반환. Screen 레이어 오픈 시 Window 레이어를 먼저 닫는다. */
	UUserWidget* AddWidget(EUIType UIType);

	/** UIType으로 테이블 조회 → 위젯 생성 → 해당 레이어의 스크린 좌표에 추가 → 반환 */
	UUserWidget* AddWidgetAtPosition(EUIType UIType, FVector2D ScreenPosition, FVector2D Alignment = FVector2D::ZeroVector);

	/** 위젯을 현재 부모에서 제거하고 OnWidgetRemoved 브로드캐스트 */
	void RemoveWidget(UUserWidget* Widget);

	/** 지정 레이어의 모든 위젯 제거하고 닫힌 EUIType마다 OnWidgetRemoved 브로드캐스트 */
	void ClearLayer(EUILayer Layer);

	/** 모든 레이어의 위젯 제거 */
	void ClearAllLayers();

	/** 닫힌 EUIType을 알리는 델리게이트 */
	FOnWidgetRemoved OnWidgetRemoved;

protected:
	UPROPERTY()
	UPDUIRootWidget* RootUI = nullptr;

	UPROPERTY()
	TMap<EUIType, TObjectPtr<UUserWidget>> ActiveWidgets;

private:
	UCanvasPanel* GetPanelForLayer(EUILayer Layer) const;

	void CreateRootUI(TSubclassOf<UPDUIRootWidget> RootWidgetClass);
	void EnsureRootUIAddedToViewport();
	void HandlePostLoadMap(UWorld* LoadedWorld);

	void AddWidgetToLayer(UUserWidget* Widget, EUILayer Layer);
	void AddWidgetToLayerAtPosition(UUserWidget* Widget, EUILayer Layer, FVector2D ScreenPosition, FVector2D Alignment);

	void CloseLayerInternal(EUILayer Layer);

};
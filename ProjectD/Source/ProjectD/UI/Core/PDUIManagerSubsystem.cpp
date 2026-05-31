// Fill out your copyright notice in the Description page of Project Settings.

#include "PDUIManagerSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "PDUIRootWidget.h"
#include "PDGameInstance.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Engine/World.h"
#include "UObject/UObjectGlobals.h"
#include "Table/PDTableManagerSubsystem.h"

void UPDUIManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (RootUI)
	{
		return;
	}

	const UPDGameInstance* PDGI = Cast<UPDGameInstance>(GetGameInstance());
	if (!PDGI)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PD][UIManager][Initialize] GameInstance is not PDGameInstance."));
		return;
	}

	const TSoftClassPtr<UPDUIRootWidget>& RootUIClass = PDGI->RootUIClass;
	if (RootUIClass.IsNull())
	{
		UE_LOG(LogTemp, Warning, TEXT("[PD][UIManager][Initialize] RootUIClass is null."));
		return;
	}

	TSubclassOf<UPDUIRootWidget> LoadedClass = RootUIClass.LoadSynchronous();
	if (LoadedClass)
	{
		CreateRootUI(LoadedClass);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[PD][UIManager] Failed to load RootUIClass."));
	}

	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UPDUIManagerSubsystem::HandlePostLoadMap);
}

void UPDUIManagerSubsystem::Deinitialize()
{
	ActiveWidgets.Empty();
	OnWidgetRemoved.Clear();

	if (RootUI)
	{
		RootUI->RemoveFromParent();
		RootUI = nullptr;
	}

	FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this);

	Super::Deinitialize();
}

void UPDUIManagerSubsystem::CreateRootUI(TSubclassOf<UPDUIRootWidget> RootWidgetClass)
{
	if (RootUI || !RootWidgetClass)
	{
		return;
	}

	UGameInstance* GI = GetGameInstance();
	if (!GI)
	{
		return;
	}

	RootUI = CreateWidget<UPDUIRootWidget>(GI, RootWidgetClass);
}

void UPDUIManagerSubsystem::EnsureRootUIAddedToViewport()
{
	if (!RootUI)
	{
		return;
	}

	UGameInstance* GI = GetGameInstance();
	UWorld* World = GI ? GI->GetWorld() : nullptr;
	if (!World || !World->GetGameViewport())
	{
		return;
	}

	if (!RootUI->IsInViewport())
	{
		RootUI->AddToViewport(0);
	}
}

UCanvasPanel* UPDUIManagerSubsystem::GetPanelForLayer(EUILayer Layer) const
{
	if (!RootUI)
	{
		return nullptr;
	}

	switch (Layer)
	{
	case EUILayer::World:  return RootUI->GetPanelWorld();
	case EUILayer::Screen: return RootUI->GetPanelScreen();
	case EUILayer::Window: return RootUI->GetPanelWindow();
	case EUILayer::Popup:  return RootUI->GetPanelPopup();
	case EUILayer::System: return RootUI->GetPanelSystem();
	default:               return nullptr;
	}
}

UUserWidget* UPDUIManagerSubsystem::AddWidget(EUIType UIType)
{
	UGameInstance* GI = GetGameInstance();
	UPDTableManagerSubsystem* TableMgr = GI ? GI->GetSubsystem<UPDTableManagerSubsystem>() : nullptr;
	if (!TableMgr)
	{
		return nullptr;
	}

	const FPDUIBase* UIData = TableMgr->GetUIBase(UIType);
	UClass* WidgetClass = TableMgr->GetWidgetClass(UIType);
	if (!UIData || !WidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PD][UIManager] AddWidget: UIBase or WidgetClass not found for UIType %d"), (int32)UIType);
		return nullptr;
	}

	// Screen 레이어가 열릴 때 Window 레이어 위젯 전체 닫기
	if (UIData->UILayer == EUILayer::Screen)
	{
		CloseLayerInternal(EUILayer::Window);
	}

	UUserWidget* Widget = CreateWidget<UUserWidget>(GI, WidgetClass);
	if (!Widget)
	{
		return nullptr;
	}

	AddWidgetToLayer(Widget, UIData->UILayer);
	ActiveWidgets.Add(UIType, Widget);
	return Widget;
}

UUserWidget* UPDUIManagerSubsystem::AddWidgetAtPosition(EUIType UIType, FVector2D ScreenPosition, FVector2D Alignment)
{
	UGameInstance* GI = GetGameInstance();
	UPDTableManagerSubsystem* TableMgr = GI ? GI->GetSubsystem<UPDTableManagerSubsystem>() : nullptr;
	if (!TableMgr)
	{
		return nullptr;
	}

	const FPDUIBase* UIData = TableMgr->GetUIBase(UIType);
	UClass* WidgetClass = TableMgr->GetWidgetClass(UIType);
	if (!UIData || !WidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PD][UIManager] AddWidgetAtPosition: UIBase or WidgetClass not found for UIType %d"), (int32)UIType);
		return nullptr;
	}

	UUserWidget* Widget = CreateWidget<UUserWidget>(GI, WidgetClass);
	if (!Widget)
	{
		return nullptr;
	}

	AddWidgetToLayerAtPosition(Widget, UIData->UILayer, ScreenPosition, Alignment);
	ActiveWidgets.Add(UIType, Widget);
	return Widget;
}

void UPDUIManagerSubsystem::AddWidgetToLayer(UUserWidget* Widget, EUILayer Layer)
{
	if (!Widget || !RootUI)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PD][UIManager] AddWidgetToLayer failed: Widget or RootUI is null."));
		return;
	}

	EnsureRootUIAddedToViewport();

	UCanvasPanel* Panel = GetPanelForLayer(Layer);
	if (!Panel)
	{
		return;
	}

	UCanvasPanelSlot* Slot = Panel->AddChildToCanvas(Widget);
	if (Slot)
	{
		Slot->SetAnchors(FAnchors(0.f, 0.f, 1.f, 1.f));
		Slot->SetOffsets(FMargin(0.f));
	}
}

void UPDUIManagerSubsystem::AddWidgetToLayerAtPosition(UUserWidget* Widget, EUILayer Layer, FVector2D ScreenPosition, FVector2D Alignment)
{
	if (!Widget || !RootUI)
	{
		return;
	}

	EnsureRootUIAddedToViewport();

	UCanvasPanel* Panel = GetPanelForLayer(Layer);
	if (!Panel)
	{
		return;
	}

	UCanvasPanelSlot* Slot = Panel->AddChildToCanvas(Widget);
	if (Slot)
	{
		Slot->SetAnchors(FAnchors(0.f, 0.f, 0.f, 0.f));
		Slot->SetPosition(ScreenPosition);
		Slot->SetAutoSize(true);
		Slot->SetAlignment(Alignment);
	}
}

void UPDUIManagerSubsystem::RemoveWidget(UUserWidget* Widget)
{
	if (!Widget)
	{
		return;
	}

	// 위젯 포인터로 UIType을 역조회해 제거 및 브로드캐스트
	for (auto It = ActiveWidgets.CreateIterator(); It; ++It)
	{
		if (It->Value == Widget)
		{
			EUIType RemovedType = It->Key;
			It.RemoveCurrent();
			Widget->RemoveFromParent();
			OnWidgetRemoved.Broadcast(RemovedType);
			return;
		}
	}

	Widget->RemoveFromParent();
}

void UPDUIManagerSubsystem::ClearLayer(EUILayer Layer)
{
	CloseLayerInternal(Layer);
}

void UPDUIManagerSubsystem::ClearAllLayers()
{
	for (EUILayer Layer : { EUILayer::World, EUILayer::Screen, EUILayer::Window, EUILayer::Popup, EUILayer::System })
	{
		CloseLayerInternal(Layer);
	}
}

void UPDUIManagerSubsystem::CloseLayerInternal(EUILayer Layer)
{
	UGameInstance* GI = GetGameInstance();
	UPDTableManagerSubsystem* TableMgr = GI ? GI->GetSubsystem<UPDTableManagerSubsystem>() : nullptr;
	if (!TableMgr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PD][UIManager] CloseLayerInternal: TableMgr is null."));
		return;
	}

	// 해당 레이어에 속하는 EUIType 수집
	TArray<EUIType> ClosedTypes;
	for (auto& Pair : ActiveWidgets)
	{
		const FPDUIBase* UIData = TableMgr->GetUIBase(Pair.Key);
		if (UIData && UIData->UILayer == Layer)
		{
			ClosedTypes.Add(Pair.Key);
		}
	}

	// 캔버스 패널 정리
	if (UCanvasPanel* Panel = GetPanelForLayer(Layer))
	{
		Panel->ClearChildren();
	}

	// 맵에서 제거 후 브로드캐스트
	for (EUIType Type : ClosedTypes)
	{
		ActiveWidgets.Remove(Type);
		OnWidgetRemoved.Broadcast(Type);
	}
}

void UPDUIManagerSubsystem::HandlePostLoadMap(UWorld* LoadedWorld)
{
	if (!LoadedWorld)
	{
		return;
	}

	EnsureRootUIAddedToViewport();
}
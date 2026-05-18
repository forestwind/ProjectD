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
	bRootUIAddedToViewport = false;

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

	bRootUIAddedToViewport = RootUI->IsInViewport();
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

	UUserWidget* Widget = CreateWidget<UUserWidget>(GI, WidgetClass);
	if (!Widget)
	{
		return nullptr;
	}

	AddWidgetToLayer(Widget, UIData->UILayer);
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

	Widget->RemoveFromParent();
}

void UPDUIManagerSubsystem::ClearLayer(EUILayer Layer)
{
	UCanvasPanel* Panel = GetPanelForLayer(Layer);
	if (Panel)
	{
		Panel->ClearChildren();
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
// Fill out your copyright notice in the Description page of Project Settings.


#include "PDUIManagerSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "PDUIRootWidget.h"
#include "../../PDGameInstance.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Engine/World.h"
#include "UObject/UObjectGlobals.h"

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

	// 레벨 로드 후 콜백 등록
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
	if (!RootUI)
	{
		return;
	}

	// AddToViewport는 뷰포트가 준비된 뒤 첫 사용 시(EnsureRootUIAddedToViewport)에 수행
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

void UPDUIManagerSubsystem::AddWidgetToPanel2D(UUserWidget* Widget)
{
	if (!Widget)
	{
		return;
	}

	if (!RootUI)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PD][UIManager] AddWidgetToPanel2D failed: RootUI is null. Widget=%s"), Widget ? *Widget->GetName() : TEXT("null"));
		return;
	}

	EnsureRootUIAddedToViewport();

	UCanvasPanel* Panel2D = RootUI->GetPanel2D();
	if (!Panel2D)
	{
		return;
	}

	UCanvasPanelSlot* Slot = Panel2D->AddChildToCanvas(Widget);
	if (Slot)
	{
		Slot->SetAnchors(FAnchors(0.f, 0.f, 1.f, 1.f));
		Slot->SetOffsets(FMargin(0.f));
	}
}

void UPDUIManagerSubsystem::AddWidgetToPanelOverlay(UUserWidget* Widget)
{
	if (!Widget)
	{
		return;
	}

	if (!RootUI)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PD][UIManager] AddWidgetToPanelOverlay failed: RootUI is null. Widget=%s"), Widget ? *Widget->GetName() : TEXT("null"));
		return;
	}

	EnsureRootUIAddedToViewport();

	UCanvasPanel* PanelOverlay = RootUI->GetPanelOverlay();
	if (!PanelOverlay)
	{
		return;
	}

	UCanvasPanelSlot* Slot = PanelOverlay->AddChildToCanvas(Widget);
	if (Slot)
	{
		Slot->SetAnchors(FAnchors(0.f, 0.f, 1.f, 1.f));
		Slot->SetOffsets(FMargin(0.f));
	}
}

void UPDUIManagerSubsystem::ClearPanel2D()
{
	if (!RootUI)
	{
		return;
	}

	UCanvasPanel* Panel2D = RootUI->GetPanel2D();
	if (Panel2D)
	{
		Panel2D->ClearChildren();
	}
}

void UPDUIManagerSubsystem::ClearPanelOverlay()
{
	if (!RootUI)
	{
		return;
	}

	UCanvasPanel* PanelOverlay = RootUI->GetPanelOverlay();
	if (PanelOverlay)
	{
		PanelOverlay->ClearChildren();
	}
}

void UPDUIManagerSubsystem::HandlePostLoadMap(UWorld* LoadedWorld)
{
	if (!LoadedWorld)
	{
		return;
	}

	// 레벨 전환 후 새 Viewport 기준으로 RootUI 재부착
	EnsureRootUIAddedToViewport();
}



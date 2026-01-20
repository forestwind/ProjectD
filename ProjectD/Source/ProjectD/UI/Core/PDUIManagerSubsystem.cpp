// Fill out your copyright notice in the Description page of Project Settings.


#include "PDUIManagerSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "PDUIRootWidget.h"
#include "../../PDGameInstance.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

void UPDUIManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (RootUI || bRootUILoadRequested)
	{
		return;
	}

	const UPDGameInstance* PDGI = Cast<UPDGameInstance>(GetGameInstance());
	if (!PDGI)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PD][UIManager] GameInstance is not PDGameInstance."));
		return;
	}

	CachedRootUIClass = PDGI->RootUIClass;

	if (CachedRootUIClass.IsNull())
	{
		UE_LOG(LogTemp, Warning, TEXT("[PD][UIManager] RootUIClass is null. Assign it in PDGameInstance."));
		return;
	}

	// 이미 로드된 경우
	if (TSubclassOf<UPDUIRootWidget> LoadedClass = CachedRootUIClass.Get())
	{
		CreateRootUI(LoadedClass);
		return;
	}

	// Async Load
	bRootUILoadRequested = true;

	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	RootUIStreamableHandle = Streamable.RequestAsyncLoad(
		CachedRootUIClass.ToSoftObjectPath(),
		FStreamableDelegate::CreateUObject(this, &UPDUIManagerSubsystem::OnRootUIClassLoaded)
	);

	// 로드 요청이 실패(핸들 invalid)하면 재시도할 수 있도록 플래그를 원복합니다.
	if (!RootUIStreamableHandle.IsValid())
	{
		bRootUILoadRequested = false;
		UE_LOG(LogTemp, Error, TEXT("[PD][UIManager] RequestAsyncLoad failed for RootUIClass (%s)."),
			*CachedRootUIClass.ToSoftObjectPath().ToString());
	}
}
void UPDUIManagerSubsystem::Deinitialize()
{
	bRootUILoadRequested = false;

	if (RootUIStreamableHandle.IsValid())
	{
		RootUIStreamableHandle->CancelHandle();
		RootUIStreamableHandle.Reset();
	}

	CachedRootUIClass.Reset();

	if (RootUI)
	{
		RootUI->RemoveFromParent();
		RootUI = nullptr;
	}

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

	RootUI->AddToViewport(0);

	UE_LOG(LogTemp, Log, TEXT("[PD][UIManager] RootUI created: %s"), *RootUI->GetName());
}

void UPDUIManagerSubsystem::OnRootUIClassLoaded()
{
	bRootUILoadRequested = false;
	RootUIStreamableHandle.Reset();

	if (RootUI)
	{
		return;
	}

	if (!GetGameInstance())
	{
		UE_LOG(LogTemp, Warning, TEXT("[PD][UIManager] GameInstance is invalid when RootUI loaded."));
		return;
	}

	TSubclassOf<UPDUIRootWidget> LoadedClass = CachedRootUIClass.Get();
	if (!LoadedClass)
	{
		UE_LOG(LogTemp, Error, TEXT("[PD][UIManager] Failed to load RootUIClass."));
		return;
	}

	CreateRootUI(LoadedClass);
}


void UPDUIManagerSubsystem::AddWidgetToPanel2D(UUserWidget* Widget)
{
	if (!RootUI || !Widget)
	{
		return;
	}

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
	if (!RootUI || !Widget)
	{
		return;
	}

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



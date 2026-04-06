// Fill out your copyright notice in the Description page of Project Settings.

#include "Sound/PDSoundManagerSubsystem.h"
#include "DataAsset/Sound/PDSoundDataAsset.h"
#include "PDGameInstance.h"
#include "Components/AudioComponent.h"

void UPDSoundManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	EnsureBGMComponent();

	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UPDSoundManagerSubsystem::HandlePreLoadMap);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UPDSoundManagerSubsystem::HandlePostLoadMap);
}

void UPDSoundManagerSubsystem::Deinitialize()
{
	FCoreUObjectDelegates::PreLoadMap.RemoveAll(this);
	FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this);

	if (BGMComponent)
	{
		BGMComponent->Stop();
		BGMComponent->DestroyComponent();
		BGMComponent = nullptr;
	}

	CachedSoundDataAsset = nullptr;

	Super::Deinitialize();
}

void UPDSoundManagerSubsystem::PlayBGM(USoundBase* InBGM, float FadeInDuration, float FadeOutDuration)
{
	EnsureBGMComponent();
	if (!BGMComponent)
	{
		return;
	}

	if (BGMComponent->IsPlaying())
	{
		BGMComponent->FadeOut(FadeOutDuration, 0.f);
	}

	if (!InBGM)
	{
		return;
	}

	BGMComponent->SetSound(InBGM);
	BGMComponent->FadeIn(FadeInDuration, BGMVolume);
}

void UPDSoundManagerSubsystem::StopBGM(float FadeOutDuration)
{
	if (BGMComponent && BGMComponent->IsPlaying())
	{
		BGMComponent->FadeOut(FadeOutDuration, 0.f);
	}
}

void UPDSoundManagerSubsystem::PlayBGMTitle(float FadeInDuration, float FadeOutDuration)
{
	UPDSoundDataAsset* SDA = GetSoundDataAsset();
	if (!SDA || SDA->BGM_Title.IsNull())
	{
		return;
	}

	PlayBGM(SDA->BGM_Title.LoadSynchronous(), FadeInDuration, FadeOutDuration);
}

void UPDSoundManagerSubsystem::PlayBGMBattle(float FadeInDuration, float FadeOutDuration)
{
	UPDSoundDataAsset* SDA = GetSoundDataAsset();
	if (!SDA || SDA->BGM_Battle.IsNull())
	{
		return;
	}

	PlayBGM(SDA->BGM_Battle.LoadSynchronous(), FadeInDuration, FadeOutDuration);
}

void UPDSoundManagerSubsystem::SetBGMVolume(float InVolume)
{
	BGMVolume = FMath::Clamp(InVolume, 0.f, 1.f);

	if (BGMComponent && BGMComponent->IsPlaying())
	{
		BGMComponent->AdjustVolume(0.f, BGMVolume);
	}
}

void UPDSoundManagerSubsystem::EnsureBGMComponent()
{
	if (BGMComponent)
	{
		return;
	}

	UGameInstance* GI = GetGameInstance();
	if (!GI)
	{
		return;
	}

	BGMComponent = NewObject<UAudioComponent>(GI, TEXT("PD_BGMAudioComponent"));
	if (!BGMComponent)
	{
		return;
	}

	BGMComponent->bAutoActivate = false;
	BGMComponent->bIsUISound = true; // 월드 거리 감쇠 없음

	UWorld* World = GI->GetWorld();
	if (World)
	{
		BGMComponent->RegisterComponentWithWorld(World);
	}
}

UPDSoundDataAsset* UPDSoundManagerSubsystem::GetSoundDataAsset()
{
	if (CachedSoundDataAsset)
	{
		return CachedSoundDataAsset;
	}

	const UPDGameInstance* PDGI = Cast<UPDGameInstance>(GetGameInstance());
	if (!PDGI || PDGI->SoundDataAsset.IsNull())
	{
		return nullptr;
	}

	CachedSoundDataAsset = PDGI->SoundDataAsset.LoadSynchronous();
	return CachedSoundDataAsset;
}

void UPDSoundManagerSubsystem::HandlePreLoadMap(const FString& MapName)
{
	if (BGMComponent && BGMComponent->IsRegistered())
	{
		BGMComponent->UnregisterComponent();
	}
}

void UPDSoundManagerSubsystem::HandlePostLoadMap(UWorld* LoadedWorld)
{
	if (!LoadedWorld || !BGMComponent)
	{
		return;
	}

	if (!BGMComponent->IsRegistered())
	{
		BGMComponent->RegisterComponentWithWorld(LoadedWorld);
	}
}

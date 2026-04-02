// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PDSoundManagerSubsystem.generated.h"

class USoundBase;
class UAudioComponent;
class UPDSoundDataAsset;

/**
 * BGM을 관리하는 GameInstance 서브시스템
 * 레벨 전환 시에도 BGM이 유지됨
 */
UCLASS()
class PROJECTD_API UPDSoundManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	/**
	 * BGM을 재생합니다.
	 * 기존 BGM이 재생 중이면 FadeOut 후 새 BGM을 FadeIn합니다.
	 * nullptr를 전달하면 BGM을 정지합니다.
	 */
	UFUNCTION(BlueprintCallable, Category = "Sound|BGM")
	void PlayBGM(USoundBase* InBGM, float FadeInDuration = 1.f, float FadeOutDuration = 1.f);

	/** BGM을 페이드 아웃하며 정지합니다. */
	UFUNCTION(BlueprintCallable, Category = "Sound|BGM")
	void StopBGM(float FadeOutDuration = 1.f);

	/** 타이틀 BGM을 재생합니다. */
	UFUNCTION(BlueprintCallable, Category = "Sound|BGM")
	void PlayBGMTitle(float FadeInDuration = 1.f, float FadeOutDuration = 1.f);

	/** 전투 BGM을 재생합니다. */
	UFUNCTION(BlueprintCallable, Category = "Sound|BGM")
	void PlayBGMBattle(float FadeInDuration = 1.f, float FadeOutDuration = 1.f);

	/** BGM 볼륨을 설정합니다 (0.0 ~ 1.0). */
	UFUNCTION(BlueprintCallable, Category = "Sound|BGM")
	void SetBGMVolume(float InVolume);

	float GetBGMVolume() const { return BGMVolume; }

private:
	/** BGMAudioComponent가 없으면 생성합니다. */
	void EnsureBGMComponent();

	/** PDGameInstance에서 SoundDataAsset을 로드하고 캐싱합니다. */
	UPDSoundDataAsset* GetSoundDataAsset();

	/** 레벨 로드 후 BGMComponent를 새 월드에 재등록합니다. */
	void HandlePostLoadMap(UWorld* LoadedWorld);

private:
	/** BGM 전용 오디오 컴포넌트. GameInstance가 아우터이므로 레벨 전환에도 유지됨. */
	UPROPERTY(Transient)
	TObjectPtr<UAudioComponent> BGMComponent;

	/** 캐싱된 SoundDataAsset */
	UPROPERTY(Transient)
	TObjectPtr<UPDSoundDataAsset> CachedSoundDataAsset;

	float BGMVolume = 1.f;
};

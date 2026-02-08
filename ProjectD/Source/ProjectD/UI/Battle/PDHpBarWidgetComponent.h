// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "PDUIBattleHpBarWidget.h"
#include "PDHpBarWidgetComponent.generated.h"

class APDCharacter;

// 캐릭터 머리 위 HP 바 전용 위젯 컴포넌트
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTD_API UPDHpBarWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UPDHpBarWidgetComponent();

	virtual void BeginPlay() override;

	void UpdateHpBar(float Value);

	// 사용할 위젯 클래스. 미지정 시 WBP_BattleHpBar를 로드한다.
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UPDUIBattleHpBarWidget> HpBarWidgetClass;

protected:
	void ResolveAndSetWidgetClass();
};

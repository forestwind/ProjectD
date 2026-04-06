// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PDUIBattleDamageNumWidget.generated.h"

class UTextBlock;

/**
 * 피격 시 데미지 수치를 화면에 띄우는 위젯.
 */
UCLASS()
class PROJECTD_API UPDUIBattleDamageNumWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "UI|DamageNumber")
	void ShowDamage(float DamageAmount);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& AllottedGeometry, float InDeltaTime) override;

	// TextBlock 바인딩
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> TextDamage;

	// 숫자가 화면에 표시되는 시간 (초)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DamageNumber", meta = (ClampMin = "0.1"))
	float DisplayDuration = 1.5f;

	// 초당 위로 이동하는 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DamageNumber")
	float RiseSpeed = 60.f;

private:
	float ElapsedTime = 0.f;
	bool bIsActive = false;
};

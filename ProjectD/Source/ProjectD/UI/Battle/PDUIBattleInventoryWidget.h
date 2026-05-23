// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Battle/Inventory/PDBattleInventorySlot.h"
#include "PDUIBattleInventoryWidget.generated.h"

class UScrollBox;
class UWrapBox;
class UButton;
class UPDUIBattleInventoryScrollItemWidget;

/**
 * 배틀 전용 인벤토리 패널
 */
UCLASS()
class PROJECTD_API UPDUIBattleInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/** 슬롯 하나를 스크롤 목록에 추가합니다. */
	UFUNCTION(BlueprintCallable, Category = "UI")
	void AddItem(const FPDBattleInventorySlot& InSlot, int32 SlotIndex);

	/** 스크롤 목록을 전부 비웁니다. */
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ClearItems();

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnCloseButtonClicked();

protected:
	/** 스크롤 영역 (디자이너에서 동일 이름으로 배치). */
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UScrollBox> ItemListScroll;

	/** ItemListScroll 의 직계 자식 WrapBox — 아이템을 가로로 배치하다 넘치면 줄바꿈. */
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UWrapBox> ItemWrapBox;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UButton> CloseButton;

	/** BP에서 지정할 행 위젯 클래스. */
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UPDUIBattleInventoryScrollItemWidget> ItemRowClass;
};
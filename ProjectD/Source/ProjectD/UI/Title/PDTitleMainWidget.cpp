// Fill out your copyright notice in the Description page of Project Settings.


#include "PDTitleMainWidget.h"
#include "Components/Button.h"

void UPDTitleMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Btn_Background)
	{
		Btn_Background->OnClicked.AddDynamic(this, &UPDTitleMainWidget::HandleBackgroundClicked);
	}
}

void UPDTitleMainWidget::HandleBackgroundClicked()
{
	OnStartRequested.Broadcast();
}

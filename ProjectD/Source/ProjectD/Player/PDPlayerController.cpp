// Fill out your copyright notice in the Description page of Project Settings.


#include "PDPlayerController.h"

#include "Character/PDCharacter.h"
#include "PDGameModeBase.h"

void APDPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	FInputModeGameAndUI InputModeGameAndUI;
	SetInputMode(InputModeGameAndUI);
	//bShowMouseCursor = false;

	APDCharacter* PDCharacter = Cast<APDCharacter>(aPawn);
	if (APDGameModeBase* PDGameModeBase = Cast<APDGameModeBase>(GetWorld()->GetAuthGameMode()))
	{
		PDGameModeBase->AddCharacter(PDCharacter, PlayerUnitID);
	}
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "PDPlayerController.h"

#include "Character/PDCharacter.h"
#include "Battle/PDBattleGameMode.h"

void APDPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	//FInputModeGameAndUI InputModeGameAndUI;
	//SetInputMode(InputModeGameAndUI);
	//bShowMouseCursor = false;

	APDCharacter* PDCharacter = Cast<APDCharacter>(aPawn);
	if (APDBattleGameMode* PDBattleGameMode = Cast<APDBattleGameMode>(GetWorld()->GetAuthGameMode()))
	{
		PDBattleGameMode->AddCharacter(PDCharacter, PlayerUnitID);
		PDBattleGameMode->SetPlayerUnitGuid(PDCharacter->GetUnitGuid());
	}
}
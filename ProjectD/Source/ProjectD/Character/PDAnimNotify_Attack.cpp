// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PDAnimNotify_Attack.h"
#include "Character/PDCharacter.h"

void UPDAnimNotify_Attack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		if (APDCharacter* OwnerCharacter = Cast<APDCharacter>(MeshComp->GetOwner()))
		{
			OwnerCharacter->CheckAttack();
		}
	}
}
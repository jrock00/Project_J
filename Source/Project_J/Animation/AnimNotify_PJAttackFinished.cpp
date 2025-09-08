// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_PJAttackFinished.h"
#include "Character/PJCharacter.h"

UAnimNotify_PJAttackFinished::UAnimNotify_PJAttackFinished(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UAnimNotify_PJAttackFinished::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (APJCharacter* Character = Cast<APJCharacter>(MeshComp->GetOwner()))
	{
		Character->AttackFinished(ComboResetDelay);
	}
}
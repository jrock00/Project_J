// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState_PJComboWindow.h"

#include "Character/PJCharacter.h"

UAnimNotifyState_PJComboWindow::UAnimNotifyState_PJComboWindow(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UAnimNotifyState_PJComboWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animaion, float TotalDuraion, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animaion, TotalDuraion, EventReference);

	if (APJCharacter* Character = Cast<APJCharacter>(MeshComp->GetOwner()))
	{
		Character->EnableComboWindow();
	}
}

void UAnimNotifyState_PJComboWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animaion,const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animaion, EventReference);

	if (APJCharacter* Character = Cast<APJCharacter>(MeshComp->GetOwner()))
	{
		Character->DisableComboWindow();
	}
}
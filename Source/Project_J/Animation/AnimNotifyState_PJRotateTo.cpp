// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState_PJRotateTo.h"
#include "Components/PJRotationComponent.h"

void UAnimNotifyState_PJRotateTo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animaion, float TotalDuraion, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animaion, TotalDuraion, EventReference);

	if (UPJRotationComponent* RotationComponent = MeshComp->GetOwner()->GetComponentByClass<UPJRotationComponent>())
	{
		RotationComponent->ToggleShouldRotate(true);
	}
}

void UAnimNotifyState_PJRotateTo::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animaion, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animaion, EventReference);

	if (UPJRotationComponent* RotationComponent = MeshComp->GetOwner()->GetComponentByClass<UPJRotationComponent>())
	{
		RotationComponent->ToggleShouldRotate(false);
	}

}

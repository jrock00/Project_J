// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState_PJParry.h"

#include "Components/PJStateComponent.h"

void UAnimNotifyState_PJParry::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animaion, float TotalDuraion, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animaion, TotalDuraion, EventReference);
	
	if (const AActor* OwnerActor = MeshComp->GetOwner())
	{
		if (UPJStateComponent* StateComponent = OwnerActor->GetComponentByClass<UPJStateComponent>())
		{
			StateComponent->SetState(PJGameplayTags::Character_State_Parrying);
		}
	}
}

void UAnimNotifyState_PJParry::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animaion, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animaion, EventReference);

	if (const AActor* OwnerActor = MeshComp->GetOwner())
	{
		if (UPJStateComponent* StateComponent = OwnerActor->GetComponentByClass<UPJStateComponent>())
		{
			StateComponent->ClearState();
		}
	}
}

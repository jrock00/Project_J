// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState_PJWeaponCollision.h"

//#include "Components/PJCombatComponent.h"
//#include "Components/PJWeaponCollisionComponent.h"
//#include "Equipments/PJWeapon.h"
#include "Interfaces/PJCombatInterface.h"


UAnimNotifyState_PJWeaponCollision::UAnimNotifyState_PJWeaponCollision(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UAnimNotifyState_PJWeaponCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animaion, float TotalDuraion, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animaion, TotalDuraion, EventReference);

	if (AActor* OwnerActor = MeshComp->GetOwner())
	{
		/*if (const UPJCombatComponent* CombatComponent = OwnerActor->GetComponentByClass<UPJCombatComponent>())
		{
			const APJWeapon* Weapon = CombatComponent->GetMainWeapon();
			if (::IsValid(Weapon))
			{
				Weapon->GetCollision()->TurnOnCollision();
			}
		}*/
		if (IPJCombatInterface* CombatInterface = Cast<IPJCombatInterface>(OwnerActor))
		{
			CombatInterface->ActivateWeaponCollision(CollisionType);
		}
	}
}

void UAnimNotifyState_PJWeaponCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animaion, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animaion, EventReference);

	if (AActor* OwnerActor = MeshComp->GetOwner())
	{
		/*if (const UPJCombatComponent* CombatComponent = OwnerActor->GetComponentByClass<UPJCombatComponent>())
		{
			const APJWeapon* Weapon = CombatComponent->GetMainWeapon();
			if (::IsValid(Weapon))
			{
				Weapon->GetCollision()->TurnOffCollision();
			}
		}*/
		if (IPJCombatInterface* CombatInterface = Cast<IPJCombatInterface>(OwnerActor))
		{
			CombatInterface->DeactivateWeaponCollision(CollisionType);
		}
	}
}

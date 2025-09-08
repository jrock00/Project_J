// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipments/PJFistWeapon.h"
#include "Components/PJCombatComponent.h"
#include "Components/PJWeaponCollisionComponent.h"
#include "Character/PJCharacter.h"
#include "Animation/PJAnimInstance.h"

APJFistWeapon::APJFistWeapon()
{
}

void APJFistWeapon::EquipItem()
{
	CombatComponent = GetOwner()->GetComponentByClass<UPJCombatComponent>();

	if (CombatComponent)
	{
		CombatComponent->SetWeapon(this);

		if (APJCharacter* OwnerCharacter = Cast<APJCharacter>(GetOwner()))
		{
			WeaponCollision->SetWeaponMesh(OwnerCharacter->GetMesh());
			SecondWeaponCollision->SetWeaponMesh(OwnerCharacter->GetMesh());

			CombatComponent->SetCombatEnabled(true);

			if (UPJAnimInstance* Anim = Cast<UPJAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance()))
			{
				Anim->UpdateCombatMode(CombatType);
			}

			WeaponCollision->AddIgnoredActor(OwnerCharacter);
			SecondWeaponCollision->AddIgnoredActor(OwnerCharacter);
		}
	}
}

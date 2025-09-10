// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipments/PJShield.h"

#include "Components/PJCombatComponent.h"
#include "PJWeapon.h"

void APJShield::EquipItem()
{
	Super::EquipItem();

	if (const AActor* OwnerActor = GetOwner())
	{
		if (UPJCombatComponent* CombatComponent = OwnerActor->GetComponentByClass<UPJCombatComponent>())
		{
			CombatComponent->SetShield(this);

			FName AttachSocketName = UnequipSocketName;

			const APJWeapon* MainWeapon = CombatComponent->GetMainWeapon();
			if (IsValid(MainWeapon))
			{
				const ECombatType CombatType = MainWeapon->GetCombatType();
				if (CombatType == ECombatType::SwordShield)
				{
					if (CombatComponent->IsCombatEnable())
					{
						AttachSocketName = EquipSocketName;
					}
				}
			}

			AttachToOwner(AttachSocketName);
		}
	}
}

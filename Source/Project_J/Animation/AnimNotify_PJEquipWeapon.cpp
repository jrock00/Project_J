// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_PJEquipWeapon.h"

#include "../PJGameplayTags.h"
#include "Components/PJCombatComponent.h"
#include "Equipments/PJWeapon.h"
#include "AnimNotify_PJAttackFinished.h"

UAnimNotify_PJEquipWeapon::UAnimNotify_PJEquipWeapon(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UAnimNotify_PJEquipWeapon::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (const AActor* Owner = MeshComp->GetOwner())
	{
		if (UPJCombatComponent* CombatComponemt = Owner->GetComponentByClass<UPJCombatComponent>())
		{
			if (APJWeapon* MainWeapon = CombatComponemt->GetMainWeapon())
			{
				bool bCombatEnabled = CombatComponemt->IsCombatEnable();
				FName WeaponSocketName;

				if (MontageActionTag == PJGameplayTags::Character_Action_Equip)
				{
					bCombatEnabled = true;
					WeaponSocketName = MainWeapon->GetEquipSocketName();
				}
				else if (MontageActionTag == PJGameplayTags::Character_Action_Unequip)
				{
					bCombatEnabled = false;
					WeaponSocketName = MainWeapon->GetUnequipSocketName();
				}

				//AttachToPlayer �Լ����� ���� ȣ���ؾ� �Ѵ�.
				CombatComponemt->SetCombatEnabled(bCombatEnabled);
				MainWeapon->AttachToOwner(WeaponSocketName);
			}
		}
	}
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PJCombatComponent.h"
#include "Equipments/PJEquipment.h"
#include "Equipments/PJWeapon.h"
#include "Equipments/PJArmour.h"
#include "Equipments/PJShield.h"
#include "Items/PJPickupItem.h"
#include "Character/PJCharacter.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UPJCombatComponent::UPJCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPJCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPJCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPJCombatComponent::SetWeapon(APJWeapon* NewWeapon)
{
	if (::IsValid(MainWeapon))
	{
		if (APJCharacter* OwnerCharacter = Cast<APJCharacter>(GetOwner()))
		{
			/*APJPickupItem* PickupItem = GetWorld()->SpawnActorDeferred<APJPickupItem>(
				APJPickupItem::StaticClass(),
				OwnerCharacter->GetActorTransform(),
				nullptr,
				nullptr,
				ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);

			PickupItem->SetEquipmentClass(MainWeapon->GetClass());
			PickupItem->FinishSpawning(GetOwner()->GetActorTransform());*/
			SpawnPickupItem(OwnerCharacter, MainWeapon->GetClass());

			MainWeapon->Destroy();
		}

			
	}

	MainWeapon = NewWeapon;
}
void UPJCombatComponent::SetShield(APJShield* NewShield)
{
	if (::IsValid(Shield))
	{
		if (const AActor* OwnerActor = GetOwner())
		{
			SpawnPickupItem(OwnerActor, Shield->GetClass());

			Shield->Destroy();
		}
	}

	Shield = NewShield;
}

void UPJCombatComponent::SetArmour(APJArmour* NewArmour)
{
	const EPJArmourType ArmourType = NewArmour->GetArmourType();

	if (APJArmour* EquippedArmourPart = GetArmour(ArmourType))
	{
		if (IsValid(EquippedArmourPart))
		{
			if (const AActor* OwnerActor = GetOwner())
			{
				SpawnPickupItem(OwnerActor, EquippedArmourPart->GetClass());
				/*APJPickupItem* PickupItem = GetWorld()->SpawnActorDeferred<APJPickupItem>(
					APJPickupItem::StaticClass(),
					OwnerActor->GetActorTransform(),
					nullptr,
					nullptr,
					ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);

				PickupItem->SetEquipmentClass(EquippedArmourPart->GetClass());
				PickupItem->FinishSpawning(GetOwner()->GetActorTransform());*/
			}
			EquippedArmourPart->UnequipItem();
			EquippedArmourPart->Destroy();
		}
		ArmourMap[ArmourType] = NewArmour;
	}
	else
	{
		ArmourMap.Add(ArmourType, NewArmour);
	}

}
void UPJCombatComponent::SetCombatEnabled(const bool bEnabled)
{
	bCombatEnabled = bEnabled;
	if (OnChangedCombat.IsBound())
	{
		OnChangedCombat.Broadcast(bCombatEnabled); 
	}
}

void UPJCombatComponent::SpawnPickupItem(const AActor* OwnerActor, const TSubclassOf<APJEquipment>& NewEquipmentClass) const
{
	APJPickupItem* PickupItem = GetWorld()->SpawnActorDeferred<APJPickupItem>(
		APJPickupItem::StaticClass(),
		OwnerActor->GetActorTransform(),
		nullptr,
		nullptr,
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);

	PickupItem->SetEquipmentClass(NewEquipmentClass);
	PickupItem->FinishSpawning(OwnerActor->GetActorTransform());
}

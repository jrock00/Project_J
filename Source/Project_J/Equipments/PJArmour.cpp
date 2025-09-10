// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipments/PJArmour.h"
#include "GameFramework/Character.h"
#include "Components/PJCombatComponent.h"
#include "Components/PJAttributeComponent.h"
#include "Character/PJCharacter.h"

APJArmour::APJArmour()
{
	if (USkeletalMeshComponent* SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("ArmourMesh"))
	{
		Mesh->DestroyComponent();
		Mesh = SkeletalMesh;
		RootComponent = Mesh;
	}
}

void APJArmour::EquipItem()
{
	Super::EquipItem();

	if (UPJCombatComponent* CombatComponent = GetOwner()->GetComponentByClass<UPJCombatComponent>())
	{
		SetBodyPartActive(false);

		CombatComponent->SetArmour(this);
		AttachToOwner(NAME_None);
	}

	if (UPJAttributeComponent* AttributeComponent = GetOwner()->GetComponentByClass<UPJAttributeComponent>())
	{
		AttributeComponent->IncreasDefense(DefenseAmount);
	}
}

void APJArmour::UnequipItem()
{
	Super::UnequipItem();

	if (USkeletalMeshComponent* SkeletalMesh = Cast<USkeletalMeshComponent>(Mesh))
	{
		SetBodyPartActive(true);

		SkeletalMesh->SetLeaderPoseComponent(nullptr);
	}

	if (UPJAttributeComponent* AttributeComponent = GetOwner()->GetComponentByClass<UPJAttributeComponent>())
	{
		AttributeComponent->DecreasDefense(DefenseAmount);
	}

	DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
}

void APJArmour::AttachToOwner(FName SocketName)
{
	Super::AttachToOwner(SocketName);

	if (USkeletalMeshComponent* SkeletalMesh = Cast<USkeletalMeshComponent>(Mesh))
	{
		if (const ACharacter* Character = Cast<ACharacter>(GetOwner()))
		{
			SkeletalMesh->SetLeaderPoseComponent(Character->GetMesh());
		}
	}
}

void APJArmour::SetBodyPartActive(const bool bActive) const
{
	if (const APJCharacter* Character = Cast< APJCharacter>(GetOwner()))
	{
		Character->SetBodyPartActive(ArmourType, bActive);
	}
}

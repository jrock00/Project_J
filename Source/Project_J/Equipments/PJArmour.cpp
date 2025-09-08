// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipments/PJArmour.h"
#include "GameFramework/Character.h"
#include "Components/PJCombatComponent.h"

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
		CombatComponent->SetArmour(this);
		AttachToOwner(NAME_None);
	}
}

void APJArmour::UnequipItem()
{
	Super::UnequipItem();

	if (USkeletalMeshComponent* SkeletalMesh = Cast<USkeletalMeshComponent>(Mesh))
	{
		SkeletalMesh->SetLeaderPoseComponent(nullptr);
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

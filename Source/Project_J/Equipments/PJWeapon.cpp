// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipments/PJWeapon.h"
#include "Animation/PJAnimInstance.h"
#include "Character/PJCharacter.h"
#include "../PJGameplayTags.h"
#include "Components/PJCombatComponent.h"
#include "Data/PJMontageActionData.h"
#include "Components/PJWeaponCollisionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

APJWeapon::APJWeapon()
{
	WeaponCollision = CreateDefaultSubobject<UPJWeaponCollisionComponent>("WeaponCollision");
	WeaponCollision->OnHitActor.AddUObject(this, &ThisClass::OnHitActor);

	SecondWeaponCollision = CreateDefaultSubobject<UPJWeaponCollisionComponent>("SecondCollision");
	SecondWeaponCollision->OnHitActor.AddUObject(this, &ThisClass::OnHitActor);

	StaminaCostMap.Add(PJGameplayTags::Character_Attack_Light, 7.f);
	StaminaCostMap.Add(PJGameplayTags::Character_Attack_Running, 12.f);
	StaminaCostMap.Add(PJGameplayTags::Character_Attack_Special, 15.f);
	StaminaCostMap.Add(PJGameplayTags::Character_Attack_Heavy, 20.f);

	DamageMultiplierMap.Add(PJGameplayTags::Character_Attack_Heavy, 1.8f);
	DamageMultiplierMap.Add(PJGameplayTags::Character_Attack_Running, 1.8f);
	DamageMultiplierMap.Add(PJGameplayTags::Character_Attack_Special, 2.1f);
}

void APJWeapon::EquipItem()
{
	Super::EquipItem();

	CombatComponent = GetOwner()->GetComponentByClass<UPJCombatComponent>();

	if (CombatComponent)
	{
		CombatComponent->SetWeapon(this);

		const FName AttachSocket = CombatComponent->IsCombatEnable() ? EquipSocketName : UnequipSocketName;

		AttachToOwner(AttachSocket);

		//������ �浹 Ʈ���̽�
		WeaponCollision->SetWeaponMesh(Mesh);

		if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
		{
			if (UPJAnimInstance* Anim = Cast<UPJAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance()))
			{
				Anim->UpdateCombatMode(CombatType);
			}
		}

		//���⸦ ������ OwnerActor �浹 ����
		WeaponCollision->AddIgnoredActor(GetOwner());
	}
}

UAnimMontage* APJWeapon::GetMontageForTag(const FGameplayTag& Tag, const int32 Index) const
{
	return MontageActionData->GetMontageForTag(Tag,Index);
}

UAnimMontage* APJWeapon::GetRandomMontageForTag(const FGameplayTag& Tag) const
{
	return MontageActionData->GetRandomMontageForTag(Tag);
}

UAnimMontage* APJWeapon::GetHitReactMontage(const AActor* Attacker) const
{
	//lookat ȸ����
//���� Actor�� �����ڸ� �ٶ󺸴� ȸ����
	const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Attacker->GetActorLocation());
	//���� Actor�� ȸ������ LooAt ȸ������ ����
	const FRotator DeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(GetActorRotation(), LookAtRotation);
	// z�� ������ ȸ���� ���̸��� ����
	const float DeltaZ = DeltaRotation.Yaw;

	EHitDirection HitDirection = EHitDirection::Front;

	if (UKismetMathLibrary::InRange_FloatFloat(DeltaZ, -45.f, 45.f))
	{
		HitDirection = EHitDirection::Front;
		UE_LOG(LogTemp, Log, TEXT("Front"));
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(DeltaZ, 45.f, 135.f))
	{
		HitDirection = EHitDirection::Left;
		UE_LOG(LogTemp, Log, TEXT("Left"));
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(DeltaZ, 135.f, 180.f)
		|| UKismetMathLibrary::InRange_FloatFloat(DeltaZ, -180.f, -135.f))
	{
		HitDirection = EHitDirection::Back;
		UE_LOG(LogTemp, Log, TEXT("Back"));
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(DeltaZ, -135.f, -45.f))
	{
		HitDirection = EHitDirection::Right;
		UE_LOG(LogTemp, Log, TEXT("Right"));
	}

	UAnimMontage* SelectedMontage = nullptr;
	switch (HitDirection)
	{
	case EHitDirection::Front:
		SelectedMontage = GetMontageForTag(PJGameplayTags::Character_Action_HitReaction, 0);
		break;
	case EHitDirection::Back:
		SelectedMontage = GetMontageForTag(PJGameplayTags::Character_Action_HitReaction, 1);
		break;
	case EHitDirection::Left:
		SelectedMontage = GetMontageForTag(PJGameplayTags::Character_Action_HitReaction, 2);
		break;
	case EHitDirection::Right:
		SelectedMontage = GetMontageForTag(PJGameplayTags::Character_Action_HitReaction, 3);
		break;
	default:
		break;
	}


	return SelectedMontage;
}

float APJWeapon::GetStaminaCost(const FGameplayTag& InTag) const
{
	if (StaminaCostMap.Contains(InTag))
	{
		return StaminaCostMap[InTag];
	}
	return 0.0f;
}

float APJWeapon::GatAttackDamage() const
{
	if (const AActor* OwnerActor = GetOwner())
	{
		const FGameplayTag LastAttackType = CombatComponent->GetLastAttackType();

		if (DamageMultiplierMap.Contains(LastAttackType))
		{
			const float Multiplier = DamageMultiplierMap[LastAttackType];
			return BaseDamame * Multiplier;
		}
	}
	return BaseDamame;
}

void APJWeapon::ActiveateCollision(EWeaponCollisionType InCollisionType)
{
	switch (InCollisionType)
	{
	case EWeaponCollisionType::MainCollision:
		WeaponCollision->TurnOnCollision();
		break;
	case EWeaponCollisionType::SecondCollision:
		SecondWeaponCollision->TurnOnCollision();
		break;
	default:
		break;
	}
}

void APJWeapon::DeactiveateCollision(EWeaponCollisionType InCollisionType)
{
	switch (InCollisionType)
	{
	case EWeaponCollisionType::MainCollision:
		WeaponCollision->TurnOffCollision();
		break;
	case EWeaponCollisionType::SecondCollision:
		SecondWeaponCollision->TurnOffCollision();
		break;
	default:
		break;
	}
}

void APJWeapon::OnHitActor(const FHitResult& Hit)
{
	AActor* TargetActor = Hit.GetActor();

	//������ ����
	FVector DamageDirection = GetOwner()->GetActorForwardVector();

	//������
	float AttackDamage = GatAttackDamage();

	UGameplayStatics::ApplyPointDamage(
		TargetActor,
		AttackDamage,
		DamageDirection,
		Hit,
		GetOwner()->GetInstigatorController(),
		this,
		nullptr
		);
}

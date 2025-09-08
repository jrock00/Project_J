// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../PJDefine.h"
#include "Equipments/PJEquipment.h"
#include "GameplayTagContainer.h"
#include "PJWeapon.generated.h"

class UPJWeaponCollisionComponent;
class UPJMontageActionData;
struct FGameplayTag;
class UPJCombatComponent;
/**
 * 
 */
UCLASS()
class PROJECT_J_API APJWeapon : public APJEquipment
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment | Socket")
	FName EquipSocketName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment | Socket")
	FName UnequipSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	ECombatType CombatType = ECombatType::SwordShield;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment | Animation")
	UPJMontageActionData* MontageActionData;

protected:
	UPROPERTY(VisibleAnywhere)
	UPJWeaponCollisionComponent* WeaponCollision;

	UPROPERTY(VisibleAnywhere)
	UPJWeaponCollisionComponent* SecondWeaponCollision;

protected:
	UPROPERTY()
	UPJCombatComponent* CombatComponent;

protected:
	UPROPERTY(EditAnywhere)
	TMap<FGameplayTag, float> StaminaCostMap;

	UPROPERTY(EditAnywhere)
	float BaseDamame = 15.f;

	//µ¥¹ÌÁö ½Â¼ö
	UPROPERTY(EditAnywhere)
	TMap<FGameplayTag, float> DamageMultiplierMap;

public:
	APJWeapon(); 

public:
	virtual void EquipItem() override;

	UAnimMontage* GetMontageForTag(const FGameplayTag& Tag, const int32 Index = 0) const;
	UAnimMontage* GetRandomMontageForTag(const FGameplayTag& Tag) const;
	UAnimMontage* GetHitReactMontage(const AActor* Attacker) const;

	float GetStaminaCost(const FGameplayTag& InTag) const;
	float GatAttackDamage() const;

	FORCEINLINE FName GetEquipSocketName() const { return EquipSocketName; };
	FORCEINLINE FName GetUnequipSocketName() const { return UnequipSocketName; };
	FORCEINLINE UPJWeaponCollisionComponent* GetCollision() const { return WeaponCollision; };
	FORCEINLINE ECombatType GetCombatType() const { return CombatType; };

public:
	virtual void ActiveateCollision(EWeaponCollisionType InCollisionType);
	virtual void DeactiveateCollision(EWeaponCollisionType InCollisionType);

public:
	void OnHitActor(const FHitResult& Hit);
};

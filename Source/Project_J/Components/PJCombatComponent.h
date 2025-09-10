// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PJDefine.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "PJCombatComponent.generated.h"

class APJWeapon;
class APJArmour;
class APJShield;
class APJEquipment;

DECLARE_MULTICAST_DELEGATE_OneParam(FDelegateOnChangedCombat, bool);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECT_J_API UPJCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FDelegateOnChangedCombat OnChangedCombat;

protected:
	UPROPERTY();
	APJWeapon* MainWeapon;

	UPROPERTY();
	APJShield* Shield;

	UPROPERTY();
	TMap<EPJArmourType, APJArmour*> ArmourMap;

	UPROPERTY(EditAnywhere)
	bool bCombatEnabled = false;

	UPROPERTY(VisibleAnywhere)
	FGameplayTag LastAttackType;

public:
	UPJCombatComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void SetWeapon(APJWeapon* NewWeapon);
	void SetShield(APJShield* NewShield);
	void SetArmour(APJArmour* NewArmour);

public:
	FORCEINLINE bool IsCombatEnable() const { return bCombatEnabled; }
	//FORCEINLINE void SetCombatEnabled(const bool bEnabled) { bCombatEnabled = bEnabled; }
	void SetCombatEnabled(const bool bEnabled);

	FORCEINLINE APJWeapon* GetMainWeapon() const { return MainWeapon; }
	FORCEINLINE APJShield* GetShield() const { return Shield; }
	FORCEINLINE APJArmour* GetArmour(const EPJArmourType ArmourType) const 
	{ 
		if (ArmourMap.Contains(ArmourType))
		{
			return ArmourMap[ArmourType];
		}
		return nullptr;
	}

	FORCEINLINE FGameplayTag GetLastAttackType() const { return LastAttackType; };
	FORCEINLINE void SetLastAttackType(const FGameplayTag& NewAttackTag) { LastAttackType = NewAttackTag; };

private:
	void SpawnPickupItem(const AActor* OwnerActor, const TSubclassOf<APJEquipment>& NewEquipmentClass) const;
};

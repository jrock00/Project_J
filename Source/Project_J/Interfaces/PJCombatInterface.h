// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../PJDefine.h"
#include "PJCombatInterface.generated.h"

struct FGameplayTag;

// This class does not need to be modified.
UINTERFACE()
class UPJCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECT_J_API IPJCombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void ActivateWeaponCollision(EWeaponCollisionType WeaponCollisionType) = 0;
	virtual void DeactivateWeaponCollision(EWeaponCollisionType WeaponCollisionType) = 0;

	virtual void PerformAttack(FGameplayTag& AttackTypeTag, FOnMontageEnded& MontageEndedDelegate) {}
};

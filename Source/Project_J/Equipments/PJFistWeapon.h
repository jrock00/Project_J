// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Equipments/PJWeapon.h"
#include "PJFistWeapon.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_J_API APJFistWeapon : public APJWeapon
{
	GENERATED_BODY()

public:
	APJFistWeapon();

public:
	virtual void EquipItem() override;
	
};

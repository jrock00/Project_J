// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PJDefine.h"
#include "Equipments/PJEquipment.h"
#include "PJArmour.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_J_API APJArmour : public APJEquipment
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment | Armour")
	EPJArmourType ArmourType = EPJArmourType::Chest;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment | Armour")
	float DefenseAmount = 0.f;

public:
	APJArmour();

	virtual void EquipItem() override;
	virtual void UnequipItem() override;
	virtual void AttachToOwner(FName SocketName) override;

	FORCEINLINE EPJArmourType GetArmourType() {return ArmourType;}

	void SetBodyPartActive(const bool bActive) const;
	
};

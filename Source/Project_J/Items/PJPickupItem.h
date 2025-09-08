// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/PJInteract.h"
#include "PJPickupItem.generated.h"

class APJEquipment;

UCLASS()
class PROJECT_J_API APJPickupItem : public AActor, public IPJInteract
{
	GENERATED_BODY()
	
public:	
	APJPickupItem();

protected:
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform);

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void Interact(AActor* Interactor) override;

public:
	FORCEINLINE void SetEquipmentClass(const TSubclassOf<APJEquipment>& NewEquipmentClass) { EquipmentClass = NewEquipmentClass; };

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item")
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TSubclassOf<APJEquipment> EquipmentClass;
};

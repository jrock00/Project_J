// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PJEquipment.generated.h"

UCLASS()
class PROJECT_J_API APJEquipment : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Equipment | Mesh")
	UStaticMesh* MeshAsset;

	UPROPERTY(EditAnywhere, Category = "Equipment | Mesh")
	UMeshComponent* Mesh;
	
public:	
	APJEquipment();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform& Transform) override;

public:
	virtual void EquipItem();

	virtual void UnequipItem();

	virtual void AttachToOwner(FName SocketName);

};

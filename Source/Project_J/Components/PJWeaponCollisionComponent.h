// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PJWeaponCollisionComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnHitActor, const FHitResult&)

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), meta = (DisplayName = "Weapon Collision"))
class PROJECT_J_API UPJWeaponCollisionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FOnHitActor OnHitActor;

protected:
	UPROPERTY(EditAnywhere)
	//���� ����
	FName TraceStartSocketName;

	UPROPERTY(EditAnywhere)
	//�� ����
	FName TraceEndSocketName;

protected:
	//Sphere ũ��(������)
	UPROPERTY(EditAnywhere)
	float TraceRadius = 20.f;

	//Trace ��� ObjectType
	UPROPERTY(EditAnywhere)
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;

	//�浹 ó�� ���� Actors
	UPROPERTY(EditAnywhere)
	TArray<AActor*> IgnoredActors;

	//�����
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType = EDrawDebugTrace::ForDuration;

protected:
	//������ MeshComponent
	// StaticMesh, SkeletaMesh
	UPROPERTY()
	UPrimitiveComponent* WeaponMesh;

	UPROPERTY()
	TArray<AActor*> AlreadyHitActors;

	bool bIsCollisionEnable = false;

public:	
	UPJWeaponCollisionComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void TurnOnCollision();

	void TurnOffCollision();

	void SetWeaponMesh(UPrimitiveComponent* MeshComponent);

	void AddIgnoredActor(AActor* Actor);

	void RemoveIgnoredActor(AActor* Actor);

protected:
	bool CanHitActor(AActor* Actor) const;

	void CollisionTrace();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../PJDefine.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PJTargetingComponent.generated.h"

class UCameraComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_J_API UPJTargetingComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TargetingRadius = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float FaceLockOnRotationSpeed = 20.f;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType = EDrawDebugTrace::ForDuration;

protected:
	UPROPERTY()
	ACharacter* Character;

	UPROPERTY()
	UCameraComponent* Camera;

	UPROPERTY()
	AActor* LockedTargetActor;

	bool bIsLockOn = false;

public:	
	UPJTargetingComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void ToggleLockOn();

	void SwitchingLockedOnActor(ESwitchingDirection InDirection);

	FORCEINLINE bool IsLockOn() const { return bIsLockOn; }

protected:
	void FindTargets(OUT TArray<AActor*>& OutTargetingActors) const;

	AActor* FindClosestTarget(TArray<AActor*>& InTargets, ESwitchingDirection InDirection = ESwitchingDirection::None) const;

	void OrientCamera() const;

	void OrientMovement() const;

	void FaceLockOnActor() const;

	void LockOnTarget();

	void StopLockOn();
};

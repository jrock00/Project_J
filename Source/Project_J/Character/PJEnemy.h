// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Sound/SoundCue.h"
#include "Interfaces/PJTargeting.h"
#include "Interfaces/PJCombatInterface.h"
#include "Engine/TargetPoint.h"
#include "PJEnemy.generated.h"

class UWidgetComponent;
class USphereComponent;
class UPJAttributeComponent;
class UPJStateComponent;
class UPJCombatComponent;
class UPJRotationComponent;
class APJWeapon;

//class ATargetPoint;

UCLASS()
class PROJECT_J_API APJEnemy : public ACharacter, public IPJTargeting, public IPJCombatInterface
{
	GENERATED_BODY()

protected:
	//타켓 출동 체크용
	UPROPERTY(VisibleAnywhere)
	USphereComponent* TargetingSphereComponent;

	UPROPERTY(VisibleAnywhere)
	UPJAttributeComponent* AttributeComponent;

	UPROPERTY(VisibleAnywhere)
	UPJStateComponent* StateComponent;

	UPROPERTY(VisibleAnywhere)
	UPJCombatComponent* CombatComponent;

	UPROPERTY(VisibleAnywhere)
	UPJRotationComponent* RotationComponent;

	//LockOn UI Widget
	UPROPERTY(VisibleAnywhere)
	UWidgetComponent* LockOnWidgetComponent;

	UPROPERTY(VisibleAnywhere)
	UWidgetComponent* HealthBarWidgetComponent;


protected:
	UPROPERTY(EditAnywhere, Category = "Effect")
	USoundCue* ImpactSound;

	UPROPERTY(EditAnywhere, Category = "Effect")
	UParticleSystem* ImpactParticle;

protected:
	UPROPERTY(EditAnywhere, Category = "AI | Patrol")
	TArray<ATargetPoint*> PatrolPoints;

	UPROPERTY(VisibleAnywhere, Category = "AI | Patrol")
	int32 PatrolIndex = 0;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<APJWeapon> DefaultWeaponClass;

public:
	APJEnemy();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void OnDeath();
	void OnAttributeChanged(EPJAttributeType AttributeType, float InValue);
	void SetupHealthBar();

protected:
	void ImpactEffect(const FVector& Location);
	void HitReaction(const AActor* Attacker);


public:
	// IPJTargeting을(를) 통해 상속됨
	void OnTargeted(bool bTargeted) override;
	bool CanBeTargeted() override;

public:
	// IPJCombatInterface을(를) 통해 상속됨
	void ActivateWeaponCollision(EWeaponCollisionType WeaponCollisionType) override;
	void DeactivateWeaponCollision(EWeaponCollisionType WeaponCollisionType) override;
	void PerformAttack(FGameplayTag& AttackTypeTag, FOnMontageEnded& MontageEndedDelegate) override;

	void ToggleHealthBarVisibility(bool bVisibility);

public:
	FORCEINLINE ATargetPoint* GetPatrolPoint()
	{
		return PatrolPoints.Num() >= (PatrolIndex + 1) ? PatrolPoints[PatrolIndex] : nullptr;
	}

	FORCEINLINE void IncrementPatrolIndex()
	{
		PatrolIndex = (PatrolIndex + 1) % PatrolPoints.Num();
	}
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Sound/SoundCue.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "Interfaces/PJCombatInterface.h"
#include "PJCharacter.generated.h"


class APJFistWeapon;

class UPJTargetingComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UPJPlayerHUDWidget;

class UCameraComponent;
class USpringArmComponent;

class UPJAttributeComponent;
class UPJStateComponent;
class UPJCombatComponent;

UCLASS()
class PROJECT_J_API APJCharacter : public ACharacter, public IPJCombatInterface
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	//Inpit Section
private:
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* SprintRollingAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* InteractAction;

	//전투
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* ToggleCombatAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* HeavyAttackAction;

	//LockedOn
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* LockOnTargetAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* LeftTargetAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* RightTargetAction;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	UPJAttributeComponent* AttributeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	UPJStateComponent* StateComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	UPJCombatComponent* CombatComponent;

	//LockedOn Targeting
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	UPJTargetingComponent* TargetingComponent;

protected:
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> PlayerHUDWidgetClass;

	UPROPERTY(VisibleAnywhere)
	UPJPlayerHUDWidget* PlayerHUDWidget;

//주먹 무기
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<APJFistWeapon> FistWeaponClass;

protected:
	UPROPERTY(EditAnywhere, Category = "Effect")
	USoundCue* ImpactSound;

	UPROPERTY(EditAnywhere, Category = "Effect")
	UParticleSystem* ImpactParticle;

protected:
	UPROPERTY(EditAnywhere, Category = "Sprinting")
	float SprintingSpeed = 750.f;
	
	UPROPERTY(EditAnywhere, Category = "Sprinting")
	float NormalSpeed = 500.f;

	UPROPERTY(EditAnywhere, Category = "Sprinting")
	bool bSprinting = false;

protected:
	bool bComboSequenceRunning = false;
	bool bCanComboInput = false;
	int32 ComboCounter = 0;
	bool bSavedComboInput = false;

	FTimerHandle ComboResetTimerHandle;

protected:
	UPROPERTY(EditAnywhere, Category = "Montage")
	UAnimMontage* RollingMontage;


public:
	APJCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	FORCEINLINE UPJStateComponent* GetStateComponent() const { return StateComponent; }
	bool IsDeath() const;

public:
	virtual float TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	void ImpactEffect(const FVector& Location);
	void HitReaction(const AActor* Attacker);
	void OnDeath();

protected:
	bool IsMoving() const;
	bool CanToggleCombat() const;
	FORCEINLINE bool IsSprinting() const { return bSprinting; }

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	void Sprinting();
	void StopSprint();

	void Rolling();

	void Interact();

	void ToggleCombat();
	void AutoToggleCombat();

	void Attack();
	void SpecialAttack();
	void HeavyAttack();

	void LockOnTarget();
	void LeftTarget();
	void RightTarget();

protected:
	// 현재 상태에서 수행 가능한 일반 공격
	FGameplayTag GetAttackPerform() const;

	bool CanPerFormAttack(const FGameplayTag& AttackTypeTag) const;
	void DoAttack(const FGameplayTag& AttackTypeTag);
	void ExecuteComboAttack(const FGameplayTag& AttackTypeTag);
	void ResetCombo();

public:
	void EnableComboWindow();
	void DisableComboWindow();
	void AttackFinished(const float ComboResetDelay);

public:
	// IPJCombatInterface을(를) 통해 상속됨
	void ActivateWeaponCollision(EWeaponCollisionType WeaponCollisionType) override;
	void DeactivateWeaponCollision(EWeaponCollisionType WeaponCollisionType) override;

};

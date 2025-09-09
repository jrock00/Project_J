// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PJDefine.h"
#include "Components/ActorComponent.h"
#include "PJAttributeComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FDelegateOnAttributeChanged, EPJAttributeType, float)
DECLARE_MULTICAST_DELEGATE(FOnDeath)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_J_API UPJAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	//µ®∏Æ∞‘¿Ã∆Æ
	FDelegateOnAttributeChanged OnAttributeChanged;

	FOnDeath OnDeath;

protected:
	UPROPERTY(EditAnywhere, Category = "Stamina")
	float BaseStamina = 300.0f;

	UPROPERTY(EditAnywhere, Category = "Stamina")
	float MaxStamina = 300.0f;

	UPROPERTY(EditAnywhere, Category = "Stamina")
	float StaminaRegneRate = 0.2f;

	UPROPERTY(EditAnywhere, Category = "Health")
	float BaseHealth = 100.f;

	UPROPERTY(EditAnywhere, Category = "Health")
	float MaxHealth = 100.f;

	UPROPERTY(VisibleAnywhere, Category = "Defense")
	float DefenseStat = 0.f;


	FTimerHandle StaminaRegenTimerHandle;


public:	
	UPJAttributeComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	FORCEINLINE float GetBaseStamina() const { return BaseStamina; }
	FORCEINLINE float GetMaxStamina() const { return MaxStamina; }
	FORCEINLINE float GetBaseHealth() const { return BaseHealth; }
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }

	FORCEINLINE void IncreasDefense(const float DefenseAmount) { DefenseStat += DefenseAmount; }
	FORCEINLINE void DecreasDefense(const float DefenseAmount) { DefenseStat -= DefenseAmount; }

	FORCEINLINE float GetStaminaRatio() const { return BaseStamina / MaxStamina; }
	FORCEINLINE float GetHealthRatio() const { return BaseHealth / MaxHealth; }

	bool CheckHasEnoughStamina(float StaminaCost) const;
	
	void DecreaseStamina(float StaminaCost);

	void ToggleStaminaRegeneration(bool bEnable, float StartDelay = 2.f);

	void BroadcastAttributeChanged(EPJAttributeType InAttributeTpye) const;

	void TakeDamageAmount(float DamageAmount);

private:
	void RegenerateStamina();
};

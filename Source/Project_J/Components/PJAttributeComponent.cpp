// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PJAttributeComponent.h"
#include "Components/PJStateComponent.h"
#include "../PJGameplayTags.h"

UPJAttributeComponent::UPJAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	
}


void UPJAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


void UPJAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

bool UPJAttributeComponent::CheckHasEnoughStamina(float StaminaCost) const
{
	return BaseStamina >= StaminaCost;
}

void UPJAttributeComponent::DecreaseStamina(float StaminaCost)
{
	BaseStamina = FMath::Clamp(BaseStamina - StaminaCost, 0.f, MaxStamina);

	BroadcastAttributeChanged(EPJAttributeType::Stamina);
}

void UPJAttributeComponent::ToggleStaminaRegeneration(bool bEnable, float StartDelay)
{
	if (bEnable)
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(StaminaRegenTimerHandle) == false)
		{
			GetWorld()->GetTimerManager().SetTimer(StaminaRegenTimerHandle, this, &ThisClass::RegenerateStamina, 0.1f, true, StartDelay);
		}
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(StaminaRegenTimerHandle);
	}
}

void UPJAttributeComponent::BroadcastAttributeChanged(EPJAttributeType InAttributeTpye) const
{
	if (OnAttributeChanged.IsBound())
	{
		float Ratio = 0.f;
		switch (InAttributeTpye)
		{
		case EPJAttributeType::Stamina:
			Ratio = GetStaminaRatio();
			break;
		case EPJAttributeType::Health:
			Ratio = GetHealthRatio();
			break;
		default:
			break;
		}

		OnAttributeChanged.Broadcast(InAttributeTpye, Ratio);
	}

}

void UPJAttributeComponent::TakeDamageAmount(float DamageAmount)
{
	BaseHealth = FMath::Clamp(BaseHealth - DamageAmount, 0.f, MaxHealth);

	BroadcastAttributeChanged(EPJAttributeType::Health);

	if (BaseHealth <= 0.f)
	{
		if (OnDeath.IsBound())
		{
			OnDeath.Broadcast();
		}

		if (UPJStateComponent* StateComp = GetOwner()->FindComponentByClass<UPJStateComponent>())
		{
			StateComp->SetState(PJGameplayTags::Character_State_Death);
		}
	}


}

void UPJAttributeComponent::RegenerateStamina()
{
	BaseStamina = FMath::Clamp(BaseStamina + StaminaRegneRate, 0.f, MaxStamina);

	BroadcastAttributeChanged(EPJAttributeType::Stamina);

	if (BaseStamina >= MaxStamina)
	{
		ToggleStaminaRegeneration(false);
	}
}


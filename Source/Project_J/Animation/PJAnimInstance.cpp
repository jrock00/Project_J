// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/PJAnimInstance.h"

#include "KismetAnimationLibrary.h"
#include "Character/PJCharacter.h"
#include "Components/PJStateComponent.h"
#include "Components/PJCombatComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "../PJGameplayTags.h"

UPJAnimInstance::UPJAnimInstance()
{
}


void UPJAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Character = Cast<ACharacter>(GetOwningActor());

	if (Character)
	{
		MovementComponent = Character->GetCharacterMovement();
		if (UPJCombatComponent* CombatComponent = Character->GetComponentByClass<UPJCombatComponent>())
		{
			CombatComponent->OnChangedCombat.AddUObject(this, &ThisClass::OnChangedCombat);
		}
	}

}
void UPJAnimInstance::NativeUpdateAnimation(float DeltaSeconds) 
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Character == nullptr)
	{
		return;
	}

	if (MovementComponent == nullptr)
	{
		return;
	}

	Velocity = MovementComponent->Velocity;
	GroundSpeed = Velocity.Size2D();

	bShouldMove = GroundSpeed > 3.f && MovementComponent->GetCurrentAcceleration() != FVector::ZeroVector;

	bIsFalling = MovementComponent->IsFalling();

	Direction = UKismetAnimationLibrary::CalculateDirection(Velocity, Character->GetActorRotation());
}

void UPJAnimInstance::AnimNotify_ResetMovementInput()
{
	if (APJCharacter* LocalCharacter = Cast<APJCharacter>(GetOwningActor()))
	{
		LocalCharacter->GetStateComponent()->ToggleMovementInput(true);
	}
}

void UPJAnimInstance::AnimNotify_ResetState()
{
	if (APJCharacter* LocalCharacter = Cast<APJCharacter>(GetOwningActor()))
	{
		LocalCharacter->GetStateComponent()->ClearState();
	}
}

void UPJAnimInstance::UpdateCombatMode(const ECombatType InCombatType)
{
	CombatType = InCombatType;
}

void UPJAnimInstance::OnChangedCombat(const bool bInCombatEnabled)
{
	bCombatEnabled = bInCombatEnabled;
}
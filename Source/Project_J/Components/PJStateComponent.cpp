// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PJStateComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UPJStateComponent::UPJStateComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPJStateComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPJStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPJStateComponent::ToggleMovementInput(bool bEnable, float Duration)
{
	if (bEnable)
	{
		FLatentActionInfo LatenAction;
		LatenAction.CallbackTarget = this;
		LatenAction.ExecutionFunction = "MovementInputEnableAction";
		LatenAction.Linkage = 0;
		LatenAction.UUID = 0;

		UKismetSystemLibrary::RetriggerableDelay(GetWorld(), Duration, LatenAction);
	}
	else 
	{
		bMovementInputEnable = false;
	}
}

void UPJStateComponent::MovementInputEnableAction()
{
	bMovementInputEnable = true;
	ClearState();
}

void UPJStateComponent::ClearState()
{
	if (CurrentState == PJGameplayTags::Character_State_Death)
	{
		return;
	}
	CurrentState = FGameplayTag::EmptyTag;
}

bool UPJStateComponent::IsCurrentStateEqualToAny(const FGameplayTagContainer& TagsToCheck) const
{
	return TagsToCheck.HasTagExact(CurrentState);
}


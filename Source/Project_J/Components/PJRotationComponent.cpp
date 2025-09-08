

#include "Components/PJRotationComponent.h"

#include "Kismet/KismetMathLibrary.h"

UPJRotationComponent::UPJRotationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UPJRotationComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


void UPJRotationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!TargetActor)
	{
		return;
	}

	if (!bShouldRotate)
	{
		return;
	}

	const FRotator LoockAtRotation = UKismetMathLibrary::FindLookAtRotation(GetOwner()->GetActorLocation(), TargetActor->GetActorLocation());
	GetOwner()->SetActorRotation(FRotator(0.f,LoockAtRotation.Yaw,0.f));
}


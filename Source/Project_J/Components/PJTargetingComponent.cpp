// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PJTargetingComponent.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Interfaces/PJTargeting.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/PJCharacter.h"

// Sets default values for this component's properties
UPJTargetingComponent::UPJTargetingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPJTargetingComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<ACharacter>(GetOwner());
	if (Character)
	{
		Camera = Character->GetComponentByClass<UCameraComponent>();
	}
	
}


// Called every frame
void UPJTargetingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsLockOn == false)
	{
		return;
	}

	if (IsValid(Character) == false)
	{
		return;
	}

	if (IsValid(LockedTargetActor) == false)
	{
		return;
	}

	const float Distance = FVector::Distance(Character->GetActorLocation(), LockedTargetActor->GetActorLocation());

	if (IPJTargeting* Targeting = Cast<IPJTargeting>(LockedTargetActor))
	{
		if (Targeting->CanBeTargeted() == false || Distance > TargetingRadius)
		{
			StopLockOn();
		}
		else
		{
			FaceLockOnActor();
		}
	}

}

void UPJTargetingComponent::ToggleLockOn()
{
	if (bIsLockOn)
	{
		StopLockOn();
	}
	else
	{
		LockOnTarget();
	}
}

void UPJTargetingComponent::SwitchingLockedOnActor(ESwitchingDirection InDirection)
{
	if (::IsValid(LockedTargetActor))
	{
		if (IPJTargeting* Targeting = Cast<IPJTargeting>(LockedTargetActor))
		{
			Targeting->OnTargeted(false);

			TArray<AActor*> OutTargets;
			FindTargets(OutTargets);
			AActor* TargetActor = FindClosestTarget(OutTargets, InDirection);

			if (::IsValid(TargetActor))
			{
				if (IPJTargeting* NewTargeting = Cast<IPJTargeting>(TargetActor))
				{
					LockedTargetActor = TargetActor;
					NewTargeting->OnTargeted(true);
				}
			}
			else 
			{
				StopLockOn();
			}
		}
	}
}

void UPJTargetingComponent::FindTargets(OUT TArray<AActor*>& OutTargetingActors) const
{
	TArray<FHitResult> OutHit;
	const FVector Start = Character->GetActorLocation();
	const FVector End = Start;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTpyes;
	ObjectTpyes.Add(UEngineTypes::ConvertToObjectType(COLLISION_OBJECT_TARGETING));

	TArray<AActor*> ActorsToIgnore;

	const bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(
		GetOwner(),
		Start,
		End,
		TargetingRadius,
		ObjectTpyes,
		false,
		ActorsToIgnore,
		DrawDebugType,
		OutHit,
		true
	);

	if (bHit)
	{
		for (const FHitResult& HitResult : OutHit)
		{
			AActor* HitActor = HitResult.GetActor();
			if (IPJTargeting* Targeting = Cast<IPJTargeting>(HitActor))
			{
				if (Targeting->CanBeTargeted())
				{
					OutTargetingActors.Add(HitActor);
				}
			}
		}
	}
}

AActor* UPJTargetingComponent::FindClosestTarget(TArray<AActor*>& InTargets, ESwitchingDirection InDirection) const
{
	float TargetCompareValue = 0.f;
	AActor* ClosestTarget = nullptr;

	for (const AActor* TargetActor : InTargets)
	{
		if (InDirection != ESwitchingDirection::None && LockedTargetActor == TargetActor)
		{
			continue;
		}

		FHitResult OutHit;
		const FVector Start = Camera->GetComponentLocation();
		const FVector End = TargetActor->GetActorLocation();

		TArray<AActor*> ActorsToIgnore;

		//����� ECC_Visiblity ä�ο� ���ؼ� Response�� Block���� �����Ǿ� �ִ��� Ȯ�� �ʿ�
		const bool bHit = UKismetSystemLibrary::LineTraceSingle(
			GetOwner(),
			Start,
			End,
			UEngineTypes::ConvertToTraceType(ECC_Visibility),
			false,
			ActorsToIgnore,
			DrawDebugType,
			OutHit,
			true
		);

		if (bHit)
		{
			//ī�޶��� �þ߸� �������� �ϱ� ������ ���� ����� ī�޶� ����
			//Lfet ���⿡ ���� ���� ���
			if (InDirection == ESwitchingDirection::Left)
			{
				//�񱳴�� Vector�� Normal �����̱� ������ �ݴ�� ���
				if (FVector::DotProduct(Camera->GetRightVector(), OutHit.Normal) > 0.f == false)
				{
					continue;
				}
			}

			if (InDirection == ESwitchingDirection::Right)
			{
				if (FVector::DotProduct(Camera->GetRightVector(), OutHit.Normal) < 0.f == false)
				{
					continue;
				}
			}

			AActor* HitActor = OutHit.GetActor();

			//�÷��̾�� ���� ����� Ÿ��
			//���濡 �ִ� ���� ����� Ÿ���� ã�� ���ؼ� ī�޶��� ForwardVector�� LookAt Vector�� �����մϴ�.
			const FRotator LoookAtRotaion = UKismetMathLibrary::FindLookAtRotation(Character->GetActorLocation(),
				HitActor->GetActorLocation());
			float CheckValue = FVector::DotProduct(Camera->GetForwardVector(), LoookAtRotaion.Vector());

			if (CheckValue > TargetCompareValue)
			{
				TargetCompareValue = CheckValue;
				ClosestTarget = HitActor;
			}
		}
	}

	return ClosestTarget;
}

void UPJTargetingComponent::OrientCamera() const
{
	Character->GetCharacterMovement()->bOrientRotationToMovement = false;
	Character->GetCharacterMovement()->bUseControllerDesiredRotation = true;
}

void UPJTargetingComponent::OrientMovement() const
{
	Character->GetCharacterMovement()->bOrientRotationToMovement = true;
	Character->GetCharacterMovement()->bUseControllerDesiredRotation = false;
}

void UPJTargetingComponent::FaceLockOnActor() const
{
	const FRotator CurrentControlRotation = Character->GetControlRotation();

	const FVector TargetLocation = LockedTargetActor->GetActorLocation() - FVector(0.f, 0.f, 150.f);
	const FRotator TargetLookAtRotation = UKismetMathLibrary::FindLookAtRotation(Character->GetActorLocation(), TargetLocation);

	const FRotator InterpRotation = FMath::RInterpTo(CurrentControlRotation, TargetLookAtRotation, GetWorld()->GetDeltaSeconds(), FaceLockOnRotationSpeed);

	Character->GetController()->SetControlRotation(FRotator(InterpRotation.Pitch, InterpRotation.Yaw, CurrentControlRotation.Roll));
}

void UPJTargetingComponent::LockOnTarget()
{
	TArray<AActor*> OutTargets;
	FindTargets(OutTargets);
	AActor* TargetActor = FindClosestTarget(OutTargets);

	if (::IsValid(TargetActor))
	{
		LockedTargetActor = TargetActor;
		bIsLockOn = true;
		if (IPJTargeting* Targeting = Cast<IPJTargeting>(LockedTargetActor))
		{
			Targeting->OnTargeted(bIsLockOn);
		}
		OrientCamera();
	}
}

void UPJTargetingComponent::StopLockOn()
{
	bIsLockOn = false;
	if (IPJTargeting* Targeting = Cast<IPJTargeting>(LockedTargetActor))
	{
		Targeting->OnTargeted(bIsLockOn);
	}
	LockedTargetActor = nullptr;
	OrientMovement();
}


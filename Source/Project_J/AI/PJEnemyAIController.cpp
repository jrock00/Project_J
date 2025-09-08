// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PJEnemyAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Character/PJEnemy.h"
#include "Perception/AIPerceptionComponent.h"
#include "Components/PJRotationComponent.h"
#include "Character/PJCharacter.h"

APJEnemyAIController::APJEnemyAIController()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("AIPerception");
}

void APJEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ControlledEnemy = Cast<APJEnemy>(InPawn);

	RunBehaviorTree(BehaviorTreeAsset);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::UpdateTarget, 0.1f, true);
}

void APJEnemyAIController::OnUnPossess()
{
	ControlledEnemy = nullptr;
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	Super::OnUnPossess();
}

void APJEnemyAIController::UpdateTarget() const
{
	TArray<AActor*> OutActors;
	AIPerceptionComponent->GetKnownPerceivedActors(nullptr, OutActors);

	//APJCharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	APJCharacter* PlayerCharacter = Cast<APJCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (OutActors.Contains(PlayerCharacter))
	{
		if (!PlayerCharacter->IsDeath())
		{
			SetTarget(PlayerCharacter);
			ControlledEnemy->ToggleHealthBarVisibility(true);
		}
		else
		{
			SetTarget(nullptr);
			ControlledEnemy->ToggleHealthBarVisibility(false);
		}
	}
	else
	{
		SetTarget(nullptr);
		ControlledEnemy->ToggleHealthBarVisibility(false);
	}
}

void APJEnemyAIController::SetTarget(AActor* NewTarget) const
{
	if (IsValid(Blackboard))
	{
		Blackboard->SetValueAsObject(FName("Target"), NewTarget);
	}

	if (UPJRotationComponent* RotationComponent = ControlledEnemy->GetComponentByClass<UPJRotationComponent>())
	{
		RotationComponent->SetTargetActor(NewTarget);
	}
}

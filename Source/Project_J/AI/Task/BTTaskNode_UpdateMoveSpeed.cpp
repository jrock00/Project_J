// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTaskNode_UpdateMoveSpeed.h"


#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"

EBTNodeResult::Type UBTTaskNode_UpdateMoveSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* ControlleredPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!ControlleredPawn)
	{
		return EBTNodeResult::Failed;
	}

	if (UCharacterMovementComponent* MovementComp = ControlleredPawn->GetComponentByClass<UCharacterMovementComponent>())
	{
		MovementComp->MaxWalkSpeed = MaxWalkSpeed;
		return EBTNodeResult::Succeeded;
	}


	return EBTNodeResult::Failed;
}

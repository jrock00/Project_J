// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_FindPatrolPoint.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/PJEnemy.h"
#include "Engine/TargetPoint.h"


EBTNodeResult::Type UBTTask_FindPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* ControlleredPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!ControlleredPawn)
	{
		return EBTNodeResult::Failed;
	}

	if (APJEnemy* Character = Cast<APJEnemy>(ControlleredPawn))
	{
		//targetpoint의 Loaction을 blackboard에 저장.
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(BlackboardLocation.SelectedKeyName, Character->GetPatrolPoint()->GetActorLocation());
		Character->IncrementPatrolIndex();
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}

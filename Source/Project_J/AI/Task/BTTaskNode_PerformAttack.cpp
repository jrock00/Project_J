// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTaskNode_PerformAttack.h"

#include "AIController.h"
#include "Components/PJStateComponent.h"
#include "Interfaces/PJCombatInterface.h"

EBTNodeResult::Type UBTTaskNode_PerformAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* ControllerPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!ControllerPawn)
	{
		return EBTNodeResult::Failed;
	}

	if (IPJCombatInterface* CombatInterface = Cast<IPJCombatInterface>(ControllerPawn))
	{
		FOnMontageEnded MontageEndedDelegate;

		MontageEndedDelegate.BindLambda([this, &OwnerComp, ControllerPawn](UAnimMontage* Montage, bool bInterrupted)
			{
				UE_LOG(LogTemp, Log, TEXT("Execute MontageEndedDelegate"));

				if (::IsValid(&OwnerComp) == false)
				{
					return;
				}

				if (UPJStateComponent* StateComponent = ControllerPawn->GetComponentByClass<UPJStateComponent>())
				{
					StateComponent->ClearState();
				}

				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			});

		CombatInterface->PerformAttack(AttackTypeTag, MontageEndedDelegate);

		return EBTNodeResult::InProgress;
	}
	return EBTNodeResult::Failed;
}

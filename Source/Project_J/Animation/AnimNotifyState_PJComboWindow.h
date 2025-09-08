// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_PJComboWindow.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "Combo Window"))
class PROJECT_J_API UAnimNotifyState_PJComboWindow : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	UAnimNotifyState_PJComboWindow(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animaion, float TotalDuraion, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animaion, const FAnimNotifyEventReference& EventReference) override;

};

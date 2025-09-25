// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_PJParry.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "Parry State"))
class PROJECT_J_API UAnimNotifyState_PJParry : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animaion, float TotalDuraion, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animaion, const FAnimNotifyEventReference& EventReference) override;

};

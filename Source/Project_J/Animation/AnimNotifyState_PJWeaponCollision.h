// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PJDefine.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_PJWeaponCollision.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_J_API UAnimNotifyState_PJWeaponCollision : public UAnimNotifyState
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponCollisionType CollisionType = EWeaponCollisionType::MainCollision;

public:
	UAnimNotifyState_PJWeaponCollision(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animaion, float TotalDuraion, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animaion, const FAnimNotifyEventReference& EventReference) override;

	
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/PJMontageActionData.h"

UAnimMontage* UPJMontageActionData::GetMontageForTag(const FGameplayTag& GroupTag, const int32 Index) const
{
	if (MontageGroupMap.Contains(GroupTag))
	{
		const FPJMontageGroup& MontageGroup = MontageGroupMap[GroupTag];

		if (MontageGroup.Animations.Num() > 0 && MontageGroup.Animations.Num() > Index)
		{
			return MontageGroup.Animations[Index];
		}
	}
	return nullptr;
}

UAnimMontage* UPJMontageActionData::GetRandomMontageForTag(const FGameplayTag& GroupTag) const
{
	if (MontageGroupMap.Contains(GroupTag))
	{
		const FPJMontageGroup& MontageGroup = MontageGroupMap[GroupTag];

		const int32 RendomIndex = FMath::RandRange(0, MontageGroup.Animations.Num() - 1);

		return  GetMontageForTag(GroupTag, RendomIndex);

		/*if (MontageGroup.Animations.Num() > 0 && MontageGroup.Animations.Num() > Index)
		{
			return MontageGroup.Animations[Index];
		}*/
	}
	return nullptr;
}

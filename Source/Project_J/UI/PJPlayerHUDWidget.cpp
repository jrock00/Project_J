// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PJPlayerHUDWidget.h"

#include "PJStatBarWidget.h"
#include "Components/PJAttributeComponent.h"

UPJPlayerHUDWidget::UPJPlayerHUDWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UPJPlayerHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (APawn* OwningPawn = GetOwningPlayerPawn())
	{
		if (UPJAttributeComponent* Attribute = OwningPawn->GetComponentByClass<UPJAttributeComponent>())
		{
			Attribute->OnAttributeChanged.AddUObject(this, &ThisClass::OnAttributeChanged);
			Attribute->BroadcastAttributeChanged(EPJAttributeType::Stamina);
			Attribute->BroadcastAttributeChanged(EPJAttributeType::Health);
		}
	}
}

void UPJPlayerHUDWidget::OnAttributeChanged(EPJAttributeType AttributeType, float InValue)
{
	switch (AttributeType)
	{
	case EPJAttributeType::Stamina:
		StaminaBarWidget->SetRatio(InValue);
		break;
	case EPJAttributeType::Health:
		HealthBarWidget->SetRatio(InValue);
		break;
	default:
		break;
	}
}

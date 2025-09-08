// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PJDefine.h"
#include "Blueprint/UserWidget.h"
#include "PJPlayerHUDWidget.generated.h"

class UPJStatBarWidget;
/**
 * 
 */
UCLASS()
class PROJECT_J_API UPJPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UPJStatBarWidget* StaminaBarWidget;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UPJStatBarWidget* HealthBarWidget;
	
public:
	UPJPlayerHUDWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	virtual void NativeConstruct() override;

protected:
	void OnAttributeChanged(EPJAttributeType AttributeType, float InValue);
};

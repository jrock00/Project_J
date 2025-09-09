// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PJCharacter.h"
#include "../PJGameplayTags.h"

#include "GameplayTagContainer.h"
#include "Camera/CameraComponent.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Components/CapsuleComponent.h"
#include "Components/PJAttributeComponent.h"
#include "Components/PJStateComponent.h"
#include "Components/PJCombatComponent.h"
#include "Components/PJTargetingComponent.h"

#include "UI/PJPlayerHUDWidget.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "Interfaces/PJInteract.h"

#include "Equipments/PJWeapon.h"
#include "Equipments/PJFistWeapon.h"
#include "Engine/DamageEvents.h"





APJCharacter::APJCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);

	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;


	AttributeComponent = CreateDefaultSubobject<UPJAttributeComponent>(TEXT("Attribute"));
	StateComponent = CreateDefaultSubobject<UPJStateComponent>(TEXT("State"));
	CombatComponent = CreateDefaultSubobject<UPJCombatComponent>(TEXT("Combat"));
	TargetingComponent = CreateDefaultSubobject<UPJTargetingComponent>(TEXT("Targeting"));

	AttributeComponent->OnDeath.AddUObject(this, &ThisClass::OnDeath);
}


void APJCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (PlayerHUDWidgetClass)
	{
		PlayerHUDWidget = CreateWidget<UPJPlayerHUDWidget>(GetWorld(), PlayerHUDWidgetClass);
		if (PlayerHUDWidget)
		{
			PlayerHUDWidget->AddToViewport();
		}
	}

	//ÁÖ¸Ô ¹«±â ÀåÂø
	if (FistWeaponClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		APJFistWeapon* FistWeapon = GetWorld()->SpawnActor<APJFistWeapon>(FistWeaponClass, GetActorTransform(), SpawnParams);
		FistWeapon->EquipItem();
	}
}

void APJCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*GEngine->AddOnScreenDebugMessage(0, 1.5f, FColor::Cyan, FString::Printf(TEXT("Stamina : %f"), AttributeComponent->GetBaseStamina()));
	GEngine->AddOnScreenDebugMessage(2, 1.5f, FColor::Cyan, FString::Printf(TEXT("MaxWalkSpeed : %f"), GetCharacterMovement()->MaxWalkSpeed));*/

}

void APJCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void APJCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);

		EnhancedInputComponent->BindAction(SprintRollingAction, ETriggerEvent::Triggered, this, &ThisClass::Sprinting);
		EnhancedInputComponent->BindAction(SprintRollingAction, ETriggerEvent::Completed, this, &ThisClass::StopSprint);

		EnhancedInputComponent->BindAction(SprintRollingAction, ETriggerEvent::Canceled, this, &ThisClass::Rolling);

		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ThisClass::Interact);

		EnhancedInputComponent->BindAction(ToggleCombatAction, ETriggerEvent::Started, this, &ThisClass::ToggleCombat);

		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ThisClass::AutoToggleCombat);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Canceled, this, &ThisClass::Attack);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ThisClass::SpecialAttack);
		EnhancedInputComponent->BindAction(HeavyAttackAction, ETriggerEvent::Started, this, &ThisClass::HeavyAttack);

		EnhancedInputComponent->BindAction(LockOnTargetAction, ETriggerEvent::Started, this, &ThisClass::LockOnTarget);
		EnhancedInputComponent->BindAction(LeftTargetAction, ETriggerEvent::Started, this, &ThisClass::LeftTarget);
		EnhancedInputComponent->BindAction(RightTargetAction, ETriggerEvent::Started, this, &ThisClass::RightTarget);
	}

}

bool APJCharacter::IsDeath() const
{
	check(StateComponent);

	FGameplayTagContainer CheckTages;
	CheckTages.AddTag(PJGameplayTags::Character_State_Death);

	return StateComponent->IsCurrentStateEqualToAny(CheckTages);
}

float APJCharacter::TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (AttributeComponent)
	{
		AttributeComponent->TakeDamageAmount(ActualDamage);
		//GEngine->AddOnScreenDebugMessage(0, 1.5f, FColor::Cyan, FString::Printf(TEXT("Damaged: %f"), ActualDamage));
	}

	StateComponent->SetState(PJGameplayTags::Character_State_Hit);
	StateComponent->ToggleMovementInput(false);

	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		const FPointDamageEvent* PointDamageEvent = static_cast<const FPointDamageEvent*>(&DamageEvent);

		FVector ShotDirection = PointDamageEvent->ShotDirection;
		FVector ImpactPoint = PointDamageEvent->HitInfo.ImpactPoint;
		FVector ImpactDirection = PointDamageEvent->HitInfo.ImpactNormal;
		FVector HitLocation = PointDamageEvent->HitInfo.Location;

		ImpactEffect(ImpactPoint);

		HitReaction(EventInstigator->GetPawn());
	}

	return ActualDamage;
}

void APJCharacter::ImpactEffect(const FVector& Location)
{
	if (ImpactParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, Location);
	}
	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, Location);
	}
}
void APJCharacter::HitReaction(const AActor* Attacker)
{
	check(CombatComponent);

	if (UAnimMontage* HitReactAnimMontege = CombatComponent->GetMainWeapon()->GetHitReactMontage(Attacker))
	{
		PlayAnimMontage(HitReactAnimMontege);
	}

}
void APJCharacter::OnDeath() 
{
	if (UCapsuleComponent* CapsuleComp = GetCapsuleComponent())
	{
		CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (USkeletalMeshComponent* MeshComp = GetMesh())
	{
		MeshComp->SetCollisionProfileName("Ragdoll");
		MeshComp->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
		MeshComp->SetSimulatePhysics(true);
	}
}

bool APJCharacter::IsMoving() const
{
	if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
	{
		return MovementComponent->Velocity.Size2D() > 3.f && MovementComponent->GetCurrentAcceleration() != FVector::ZeroVector;
	}

	return false;
}

bool APJCharacter::CanToggleCombat() const
{
	check(StateComponent);

	if (IsValid(CombatComponent->GetMainWeapon()) == false)
	{
		return false;
	}

	if (CombatComponent->GetMainWeapon()->GetCombatType() == ECombatType::MeleeFists)
	{
		return false;
	}

	FGameplayTagContainer CheckTags;
	CheckTags.AddTag(PJGameplayTags::Character_State_Attacking);
	CheckTags.AddTag(PJGameplayTags::Character_State_Rolling);
	CheckTags.AddTag(PJGameplayTags::Character_State_GeneralAction);

	return StateComponent->IsCurrentStateEqualToAny(CheckTags) == false;
}

void APJCharacter::Move(const FInputActionValue& Value)
{
	check(StateComponent)

	if(StateComponent->MovementInputEnable() == false)
	{
		return;
	}

	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void APJCharacter::Look(const FInputActionValue& Value)
{
	if (TargetingComponent && TargetingComponent->IsLockOn())
	{
		return;
	}

	FVector2d LookDirection = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookDirection.X);
		AddControllerPitchInput(LookDirection.Y);
	}
}

void APJCharacter::Sprinting()
{
	if (AttributeComponent->CheckHasEnoughStamina(5.f) && IsMoving())
	{
		AttributeComponent->ToggleStaminaRegeneration(false);

		GetCharacterMovement()->MaxWalkSpeed = SprintingSpeed;

		AttributeComponent->DecreaseStamina(0.1f);

		bSprinting = true;
	}
	else
	{
		StopSprint();
	}
}

void APJCharacter::StopSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	AttributeComponent->ToggleStaminaRegeneration(true);
	bSprinting = false;
}

void APJCharacter::Rolling()
{
	check(AttributeComponent);
	check(StateComponent);

	if (AttributeComponent->CheckHasEnoughStamina(15.f))
	{
		AttributeComponent->ToggleStaminaRegeneration(false);

		StateComponent->ToggleMovementInput(false);

		AttributeComponent->DecreaseStamina(15.f);

		PlayAnimMontage(RollingMontage);

		StateComponent->SetState(PJGameplayTags::Character_State_Rolling);

		AttributeComponent->ToggleStaminaRegeneration(true, 1.5f);
	}
}

void APJCharacter::Interact()
{
	FHitResult OutHit;
	const FVector Start = GetActorLocation();
	const FVector End = Start;
	constexpr float Radius = 100.f;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(COLLISION_OBJECT_INTERACTION));

	TArray<AActor*> ActorsToIgnore;

	bool bHit = UKismetSystemLibrary::SphereTraceSingleForObjects(
		this,
		Start,
		End,
		Radius,
		ObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		OutHit,
		true);

	if (bHit)
	{
		if (AActor* HitActor = OutHit.GetActor())
		{
			if (IPJInteract* Interaction = Cast<IPJInteract>(HitActor))
			{
				Interaction->Interact(this);
			}
		}
	}
}

void APJCharacter::ToggleCombat()
{
	check(CombatComponent)
	check(StateComponent)

		if (CombatComponent)
		{
			if (const APJWeapon* Weapon = CombatComponent->GetMainWeapon())
			{
				if (CanToggleCombat())
				{
					StateComponent->SetState(PJGameplayTags::Character_State_GeneralAction);

					if (CombatComponent->IsCombatEnable())
					{
						PlayAnimMontage(Weapon->GetMontageForTag(PJGameplayTags::Character_Action_Unequip));
					}
					else
					{
						PlayAnimMontage(Weapon->GetMontageForTag(PJGameplayTags::Character_Action_Equip));
					}
				}
			}
		}
}

void APJCharacter::AutoToggleCombat()
{
	if (CombatComponent)
	{
		if (!CombatComponent->IsCombatEnable())
		{
			ToggleCombat();
		}
	}
}

void APJCharacter::Attack()
{
	const FGameplayTag AttackTypeTag = GetAttackPerform();

	if (CanPerFormAttack(AttackTypeTag))
	{
		ExecuteComboAttack(AttackTypeTag);
	}
}

void APJCharacter::SpecialAttack()
{
	const FGameplayTag AttackTypeTag = PJGameplayTags::Character_Attack_Special;

	if (CanPerFormAttack(AttackTypeTag))
	{
		ExecuteComboAttack(AttackTypeTag);
	}
}

void APJCharacter::HeavyAttack()
{
	AutoToggleCombat();

	const FGameplayTag AttackTypeTag = PJGameplayTags::Character_Attack_Heavy;

	if (CanPerFormAttack(AttackTypeTag))
	{
		ExecuteComboAttack(AttackTypeTag);
	}
}

void APJCharacter::LockOnTarget()
{
	TargetingComponent->ToggleLockOn();
}

void APJCharacter::LeftTarget()
{
	TargetingComponent->SwitchingLockedOnActor(ESwitchingDirection::Left);
}

void APJCharacter::RightTarget()
{
	TargetingComponent->SwitchingLockedOnActor(ESwitchingDirection::Right);
}

FGameplayTag APJCharacter::GetAttackPerform() const
{
	if (IsSprinting())
	{
		return PJGameplayTags::Character_Attack_Running;
	}
	return PJGameplayTags::Character_Attack_Light;
}

bool APJCharacter::CanPerFormAttack(const FGameplayTag& AttackTypeTag) const
{
	check(StateComponent);
	check(AttributeComponent);
	check(CombatComponent);

	if (IsValid(CombatComponent->GetMainWeapon()) == false)
	{
		return false;
	}

	FGameplayTagContainer CheckTags;
	CheckTags.AddTag(PJGameplayTags::Character_State_Rolling);
	CheckTags.AddTag(PJGameplayTags::Character_State_GeneralAction);
	CheckTags.AddTag(PJGameplayTags::Character_State_Hit);

	const float StaminaCost = CombatComponent->GetMainWeapon()->GetStaminaCost(AttackTypeTag);

	return StateComponent->IsCurrentStateEqualToAny(CheckTags) == false
		&& CombatComponent->IsCombatEnable()
		&& AttributeComponent->CheckHasEnoughStamina(StaminaCost);
}

void APJCharacter::DoAttack(const FGameplayTag& AttackTypeTag)
{
	check(StateComponent);
	check(AttributeComponent);
	check(CombatComponent);

	if (const APJWeapon* Weapon = CombatComponent->GetMainWeapon())
	{
		StateComponent->SetState(PJGameplayTags::Character_State_Attacking);
		StateComponent->ToggleMovementInput(false);
		CombatComponent->SetLastAttackType(AttackTypeTag);

		AttributeComponent->ToggleStaminaRegeneration(false);

		UAnimMontage* Montage = Weapon->GetMontageForTag(AttackTypeTag, ComboCounter);
		if (!Montage)
		{
			ComboCounter = 0;
			Montage = Weapon->GetMontageForTag(AttackTypeTag, ComboCounter);
		}

		PlayAnimMontage(Montage);

		const float StaminaCost = Weapon->GetStaminaCost(AttackTypeTag);
		AttributeComponent->DecreaseStamina(StaminaCost);
		AttributeComponent->ToggleStaminaRegeneration(true, 1.5f);
	}
}

void APJCharacter::ExecuteComboAttack(const FGameplayTag& AttackTypeTag)
{
	if (StateComponent->GetCurrentState() != PJGameplayTags::Character_State_Attacking)
	{
		if (bComboSequenceRunning && bCanComboInput == false)
		{
			ComboCounter++;
			UE_LOG(LogTemp, Warning, TEXT("Combo Counter = %d"), ComboCounter);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT(">>> ------ComboSequence Started------ <<<"));
			ResetCombo();
			bComboSequenceRunning = true;
		}

		DoAttack(AttackTypeTag);
		GetWorld()->GetTimerManager().ClearTimer(ComboResetTimerHandle);
	}
	else if (bCanComboInput)
	{
		bSavedComboInput = true;
	}
}

void APJCharacter::ResetCombo()
{
	UE_LOG(LogTemp, Warning, TEXT("Combo Reset"));

	bComboSequenceRunning = false;
	bCanComboInput = false;
	bSavedComboInput = false;
	ComboCounter = 0;
}

void APJCharacter::EnableComboWindow()
{
	bCanComboInput = true;
	UE_LOG(LogTemp, Warning, TEXT("Combo Window Opened : Combo Counter = %d"), ComboCounter);
}

void APJCharacter::DisableComboWindow()
{
	check(CombatComponent);

	bCanComboInput = false;

	if (bSavedComboInput)
	{
		bSavedComboInput = false;
		ComboCounter++;
		UE_LOG(LogTemp, Warning, TEXT("Combo Window Closed : Advencing to next combo = %d"), ComboCounter);
		DoAttack(CombatComponent->GetLastAttackType());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Combo Window Closed : No input received"));
	}
}

void APJCharacter::AttackFinished(const float ComboResetDelay)
{
	if (StateComponent)
	{
		StateComponent->ToggleMovementInput(true);
	}
	ResetCombo();
}

void APJCharacter::ActivateWeaponCollision(EWeaponCollisionType WeaponCollisionType)
{
	if (CombatComponent)
	{
		CombatComponent->GetMainWeapon()->ActiveateCollision(WeaponCollisionType);
	}
}

void APJCharacter::DeactivateWeaponCollision(EWeaponCollisionType WeaponCollisionType)
{
	if (CombatComponent)
	{
		CombatComponent->GetMainWeapon()->DeactiveateCollision(WeaponCollisionType);
	}
}


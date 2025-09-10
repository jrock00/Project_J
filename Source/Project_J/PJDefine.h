// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#define COLLISION_OBJECT_INTERACTION ECC_GameTraceChannel1
#define COLLISION_OBJECT_TARGETING ECC_GameTraceChannel2

UENUM(BlueprintType)
enum class EPJAttributeType : uint8
{
	Stamina,
	Health,
};

UENUM(BlueprintType)
enum class EHitDirection : uint8
{
	Front,
	Back,
	Left,
	Right,
};

UENUM(BlueprintType)
enum class ESwitchingDirection : uint8
{
	Right,
	Left,
	None,
};

UENUM(BlueprintType)
enum class ECombatType : uint8
{
	None,
	SwordShield,
	TwoHanded,
	MeleeFists
};

UENUM(BlueprintType)
enum class EWeaponCollisionType : uint8
{
	MainCollision,
	SecondCollision,
};

UENUM(BlueprintType)
enum class EPJAIBehavior : uint8
{
	Idle,
	Patrol,
	MeleeAttack,
	Approach,
};

UENUM(BlueprintType)
enum class EPJArmourType : uint8
{
	Chest,
	Pants,
	Boots,
	Gloves,
};

UENUM(BlueprintType)
enum class EPJEquipmentType : uint8
{
	Weapon,
	Shield,
};
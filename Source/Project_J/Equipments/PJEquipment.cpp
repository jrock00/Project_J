// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipments/PJEquipment.h"
#include "GameFramework/Character.h"

// Sets default values
APJEquipment::APJEquipment()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EquipmentMesh"));
	SetRootComponent(Mesh);
	Mesh->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
}

// Called when the game starts or when spawned
void APJEquipment::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APJEquipment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APJEquipment::OnConstruction(const FTransform& Transform) 
{
	Super::OnConstruction(Transform);

	if (UStaticMeshComponent* StaticMesh = Cast<UStaticMeshComponent>(Mesh))
	{
		StaticMesh->SetStaticMesh(MeshAsset);
	}
}

void APJEquipment::EquipItem()
{
}

void APJEquipment::UnequipItem()
{
}

void APJEquipment::AttachToOwner(FName SocketName)
{
	if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
	{
		if (USkeletalMeshComponent* CharacterMesh = OwnerCharacter->GetMesh())
		{
			AttachToComponent(CharacterMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), SocketName);
		}
	}
}


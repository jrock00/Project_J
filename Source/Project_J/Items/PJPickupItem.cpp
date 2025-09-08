// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/PJPickupItem.h"
#include "Equipments/PJEquipment.h"
#include "../PJDefine.h"


// Sets default values
APJPickupItem::APJPickupItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupItemMesh"));
	SetRootComponent(Mesh);

	Mesh->SetCollisionObjectType(COLLISION_OBJECT_INTERACTION);
	Mesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

}

// Called when the game starts or when spawned
void APJPickupItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APJPickupItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//�𸮾� �����Ϳ��� ������ �Ӽ��̳� Ʈ������ ������ ����� �� ȣ��Ǵ� �Լ��̴�.
void APJPickupItem::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (EquipmentClass)
	{
		//CDO -> Class Default Object , �� Ŭ������ �����Ǵ� ������Ʈ�� �����ø�, �������Ʈ���� ������ ������ ���� ��������
		if (APJEquipment* CDO = EquipmentClass->GetDefaultObject<APJEquipment>())
		{
			Mesh->SetStaticMesh(CDO->MeshAsset);
			Mesh->SetSimulatePhysics(true);
			Mesh->SetWorldScale3D(CDO->Mesh->GetRelativeScale3D());
		}
	}
}

void APJPickupItem::Interact(AActor* Interactor)
{
	//GEngine->AddOnScreenDebugMessage(0, 1.5f, FColor::Cyan, TEXT("Hello"));
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Interactor;

	if (APJEquipment* CDO = EquipmentClass->GetDefaultObject<APJEquipment>())
	{
		FTransform SpawnTransform = GetActorTransform();

		SpawnTransform.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

	APJEquipment* SpawnItem = GetWorld()->SpawnActor<APJEquipment>(EquipmentClass, SpawnTransform, SpawnParams);
	if (SpawnItem)
	{
		SpawnItem->EquipItem();
		Destroy();
	}
	}

}


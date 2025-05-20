// Fill out your copyright notice in the Description page of Project Settings.

#include "Map/SpawItemBase.h"
#include "Pawn/LMPawnPlayer.h"
#include "Game/GameManager.h"
#include "BuffDebuff.h"
#include "Components/SphereComponent.h"

// Sets default values
ASpawItemBase::ASpawItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	//ECurrentItemType = ELMItemType::None;
}

// Called when the game starts or when spawned
void ASpawItemBase::BeginPlay()
{
	Super::BeginPlay();
	//GenerateItemData();

	UE_LOG(LogTemp, Warning, TEXT("ASpawItemBase::BeginPlay() called"));

	auto* SphereComp = FindComponentByClass<USphereComponent>();
	if (!SphereComp)
	{
		UE_LOG(LogTemp, Error, TEXT("¢º SphereComponent NOT FOUND!"));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("¢º SphereComponent FOUND, binding overlap"));
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASpawItemBase::OnSphereOverlap);
}

// Called every frame
void ASpawItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASpawItemBase::OnSphereOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	if (auto Player = Cast<ALMPawnPlayer>(OtherActor))
	{
		switch (myItemType)
		{
		case ELMItemType::None:
			break;
		case ELMItemType::LightningAttack:
		case ELMItemType::Fireball:
		case ELMItemType::StunAttack:
		case ELMItemType::ObstacleDestroyer:
		case ELMItemType::TileTaker:
		case ELMItemType::MaxBoundary:
			Player->GetItem(myItemType);
			break;
		case ELMItemType::BuffItem:
		case ELMItemType::DebuffItem:
			OnItemCollision(Player);
			break;
		}

		Destroy();
	}
}


void ASpawItemBase::OnItemCollision(ALMPawnPlayer* AquiredPlayer)
{
	UE_LOG(LogTemp, Warning, TEXT("Test Message"));

	if (nullptr != AquiredPlayer)
	{
		AquiredPlayer->ApplyBuffDebuff();
		switch (myItemType)
		{
		case ELMItemType::BuffItem:
			AquiredPlayer->SetBuff(BuffDebuffData->GetBuffType(), BuffDebuffData);			
			break;
		case ELMItemType::DebuffItem:
			AquiredPlayer->SetDebuff(BuffDebuffData->GetDebuffType(), BuffDebuffData);			
			break;
		default:
			break;
		}
	}
}

void ASpawItemBase::GenerateItemData()
{
	int32 RandomResult = FMath::RandRange(1, 5);//(int32)ELMItemType::None + 1, (int32)ELMItemType::MaxBoundary - 1);
	myItemType = (ELMItemType)RandomResult;

	//myItemType = ELMItemType::BuffItem;

	myItemType = FMath::RandBool()
		? ELMItemType::BuffItem
		: ELMItemType::DebuffItem;

	switch (myItemType)
	{
	case ELMItemType::None:
		break;
	case ELMItemType::LightningAttack:
		break;
	case ELMItemType::Fireball:
		myItemType = ELMItemType::LightningAttack;
		break;
	case ELMItemType::StunAttack:
		break;
	case ELMItemType::ObstacleDestroyer:
		myItemType = ELMItemType::StunAttack;
		break;
	case ELMItemType::TileTaker:
		myItemType = ELMItemType::LightningAttack;
		break;
	case ELMItemType::BuffItem:
		BuffDebuffData = NewObject<UBuffDebuff>();
		BuffDebuffData->GenerateBuff();
		break;
	case ELMItemType::DebuffItem:
		BuffDebuffData = NewObject<UBuffDebuff>();
		BuffDebuffData->GenerateDebuff();
		break;
	case ELMItemType::MaxBoundary:
		break;
	default:
		break;
	}


}


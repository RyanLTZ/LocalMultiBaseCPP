// Fill out your copyright notice in the Description page of Project Settings.

#include "Map/SpawItemBase.h"
#include "Pawn/LMPawnPlayer.h"
#include "Game/GameManager.h"
#include "BuffDebuff.h"

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
}

// Called every frame
void ASpawItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASpawItemBase::OnItemCollision(ALMPawnPlayer* AquiredPlayer)
{
	if (AquiredPlayer)
	{
		AquiredPlayer->ApplyBuffDebuff();
		switch (ItemType)
		{
		case ELMItemType::BuffItem:
			AquiredPlayer->SetBuff(BuffDebuffData->GetBuffType(), BuffDebuffData);			
			break;
		case ELMItemType::DebuffItem:
			AquiredPlayer->SetBuff(BuffDebuffData->GetBuffType(), BuffDebuffData);			
			break;
		default:
			break;
		}

	}
}

void ASpawItemBase::GenerateItemData()
{
	int32 RandomResult = FMath::RandRange(1, 5);//(int32)ELMItemType::None + 1, (int32)ELMItemType::MaxBoundary - 1);
	ItemType = ELMItemType::StunAttack;// (ELMItemType)RandomResult;

	switch (ItemType)
	{
	case ELMItemType::None:
		break;
	case ELMItemType::LightningAttack:
		break;
	case ELMItemType::Fireball:
		break;
	case ELMItemType::StunAttack:
		break;
	case ELMItemType::ObstacleDestroyer:
		break;
	case ELMItemType::TileTaker:
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


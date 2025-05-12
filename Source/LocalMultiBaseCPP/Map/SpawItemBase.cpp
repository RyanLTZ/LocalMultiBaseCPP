// Fill out your copyright notice in the Description page of Project Settings.

#include "Map/SpawItemBase.h"
#include "Pawn/LMPawnPlayer.h"
#include "Game/GameManager.h"

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
	}
}


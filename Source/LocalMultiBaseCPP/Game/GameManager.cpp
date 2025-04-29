// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GameManager.h"
#include "LMGameModeBase.h"

// Sets default values
AGameManager::AGameManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	UE_LOG(LogTemp, Warning, TEXT("GameManager Created"));

}

// Called when the game starts or when spawned
void AGameManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (RemainTime > 0)
	{
		RemainTime -= DeltaTime;
	}		
	else
	{
		RemainTime = 0;
		//PrimaryActorTick.bCanEverTick = false; 
		SetActorTickEnabled(false);
		FinishGame();
	}		

}

void AGameManager::FinishGame()
{	
	FUNCDeleOnGameFinish.ExecuteIfBound();
}

int32 AGameManager::DetermineWinnerPlayer()
{
	return int32();
}

void AGameManager::SetRemainTime(float NewTime)
{
	RemainTime = NewTime;
	RemainTime > 0 ? SetActorTickEnabled(true) : SetActorTickEnabled(false);
}


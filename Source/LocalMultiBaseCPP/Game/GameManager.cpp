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
void AGameManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	FUNCDeleOnGameFinish.Unbind();
	FUNCDeleOnTimeChange.Unbind();	
}

// Called every frame
void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	

	if (RemainTime > 0)
	{
		TempElapsedTimeForTick += DeltaTime;
		RemainTime -= DeltaTime;
		if (TempElapsedTimeForTick >= UpdateTick)
		{
			TempElapsedTimeForTick = 0.f;
			FUNCDeleOnTimeChange.ExecuteIfBound(RemainTime);
		}

		ProcessItemLifeCycle(DeltaTime);
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

void AGameManager::ProcessItemLifeCycle(float DeltaTime)
{
	if (RemainTime <= 0)
		return;

	if (TempElapsedTimeForItemDestroy == 0 && TempElapsedDelayTimeForNextSpawn == 0)
	{
		TempElapsedDelayTimeForNextSpawn = ItemSpawnDelayAfterDisappear;
		return;
	}
	
	if (TempElapsedTimeForItemDestroy > 0)
	{
		TempElapsedTimeForItemDestroy += DeltaTime;

		if (TempElapsedTimeForItemDestroy >= ItemLifeTime)
		{
			TempElapsedTimeForItemDestroy = 0;
			FUNCDeleOnItemDestroy.ExecuteIfBound();
			TempElapsedDelayTimeForNextSpawn += DeltaTime;
		}	
	}

	if (TempElapsedDelayTimeForNextSpawn > 0)
	{
		TempElapsedDelayTimeForNextSpawn += DeltaTime;

		if (TempElapsedDelayTimeForNextSpawn >= ItemSpawnDelayAfterDisappear)
		{
			TempElapsedDelayTimeForNextSpawn = 0;
			FUNCDeleOnItemSpawn.ExecuteIfBound();			
			
			TempElapsedTimeForItemDestroy += DeltaTime;
		}
	}
	
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


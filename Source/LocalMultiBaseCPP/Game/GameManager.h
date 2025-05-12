// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameManager.generated.h"



DECLARE_DELEGATE(FDeleOnGameFinish);
DECLARE_DELEGATE_OneParam(FDeleOnTimeChange, float);
DECLARE_DELEGATE(FDeleOnItemSpawn);
DECLARE_DELEGATE(FDeleOnItemDestroy);


UCLASS()
class LOCALMULTIBASECPP_API AGameManager : public AActor
{
	GENERATED_BODY()	

public:	
	// Sets default values for this actor's properties
	AGameManager();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	//User Added Variables
private:
	UPROPERTY(VisibleAnywhere)
	float RemainTime;	
	
	UPROPERTY(EditAnywhere, Category = "ItemSpawn");
	float ItemLifeTime = 5;

	UPROPERTY(EditAnywhere, Category = "ItemSpawn");
	float ItemSpawnDelayAfterDisappear = 1.5f;

	const float UpdateTick = 0.1f;
	float TempElapsedTimeForTick = 0.f;
	float TempElapsedTimeForItemDestroy = 0.f;
	float TempElapsedDelayTimeForNextSpawn = 0.f;

public:
	FDeleOnGameFinish FUNCDeleOnGameFinish;
	FDeleOnTimeChange FUNCDeleOnTimeChange;
	FDeleOnItemSpawn FUNCDeleOnItemSpawn;
	FDeleOnItemDestroy FUNCDeleOnItemDestroy;
	//user Addeded Functions 
private:
	void FinishGame();
	void ProcessItemLifeCycle(float DeltaTime);

public:
	int32 DetermineWinnerPlayer();
	void SetRemainTime(float NewTime);	
	const float GetRemainTime() { return RemainTime; }				

};

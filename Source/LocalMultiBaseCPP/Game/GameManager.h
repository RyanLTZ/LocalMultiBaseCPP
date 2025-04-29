// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameManager.generated.h"

DECLARE_DELEGATE(FDeleOnGameFinish);

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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	//User Added Variables
private:
	UPROPERTY(VisibleAnywhere)
	float RemainTime;	
public:
	FDeleOnGameFinish FUNCDeleOnGameFinish;

	//user Addeded Functions 
private:
	void FinishGame();

public:
	int32 DetermineWinnerPlayer();
	void SetRemainTime(float NewTime);	
	const float GetRemainTime() { return RemainTime; }			

};

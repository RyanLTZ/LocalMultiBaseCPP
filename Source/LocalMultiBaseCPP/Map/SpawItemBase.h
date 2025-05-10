// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawItemBase.generated.h"


UCLASS()
class LOCALMULTIBASECPP_API ASpawItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawItemBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;	

	UFUNCTION(BlueprintCallable)
	void OnItemCollision(class ALMPawnPlayer* AquiredPlayer );

};
 
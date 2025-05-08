// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Map/Obstacle.h"
#include "DestructableObst.generated.h"

/**
 * 
 */
UCLASS()
class LOCALMULTIBASECPP_API ADestructableObst : public AObstacle
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:
	void SpawnOther();
};

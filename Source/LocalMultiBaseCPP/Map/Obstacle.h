// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Map/TileBase.h"
#include "Obstacle.generated.h"

/**
 * 
 */
UCLASS()
class LOCALMULTIBASECPP_API AObstacle : public ATileBase
{
	GENERATED_BODY()

public:
	AObstacle();

public:
	void SetBorderSizeHorizontalAxisByTileCount(int32 CountInWidth, const FVector TileSize);
	void SetBorderSizeVerticalAxisByTileCount(int32 CountInLength, const FVector TileSize);
	void SetTransitable();
	
};

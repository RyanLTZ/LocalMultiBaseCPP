// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TileGenerator.generated.h"


UCLASS()
class LOCALMULTIBASECPP_API ATileGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATileGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	
	TArray<class ATileBase*> ArrayTileBaseRow;
	TArray<TArray<class ATileBase*>> ArrayOfTileRow;
	//TArray<class ATileBase*> ArrayTileBase;
	
	

	UPROPERTY(EditAnywhere, Category = "Map");
	TSubclassOf<class ATileBase> TileBaseClass;

	int32 TileCountWidth = 0;
	int32 TileCountLength = 0;
	FVector CurrentActorLocation;

public:
	void ClearTileList();

	UFUNCTION(BlueprintCallable, Category = "Map")
	void GenerateMap(int32 CountWidthDir,  int32 CountLengthDir);
	
	void SetTilePosition(const TArray<ATileBase*> InputArray);
	FVector GetTilePosition(const int32 ArrIndex, const int32 TotalCountInWidth, const int32 TotalCountInLength);
	
	
};

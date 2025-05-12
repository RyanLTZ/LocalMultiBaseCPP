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
	TArray<class ASpawItemBase*> ArrayItem;
	TMap<int32, ATileBase*> MapTileList;
	//TMap<int32, class ASpawItemBase*> MapItemList;

	UPROPERTY(EditAnywhere, Category = "Map")
	TSubclassOf<class ATileBase> TileBaseClass;

	UPROPERTY(EditAnywhere, Category = "Map")
	TSubclassOf<class AObstacle> ObstacleClass;

	UPROPERTY(EditAnywhere, Category = "Map")
	TSubclassOf<class ADestructableObst> DestructableObsClass;

	UPROPERTY(EditAnywhere, Category = "Map")
	TSubclassOf<class ARandomSpawnObject> RandomSpawnObjectClass;

	UPROPERTY(EditAnywhere, Category = "Map")
	TSubclassOf<class ARandomSpawnObject> RandomSpawnObjectType2Class;


	UPROPERTY(EditAnywhere, Category = "Item")
	TSubclassOf<class ASpawItemBase> SpawnItemBaseClass;

	UPROPERTY(EditAnywhere, Category = "InputParams")
	int32 TileCountWidth = 0;

	UPROPERTY(EditAnywhere, Category = "InputParams")
	int32 TileCountLength = 0;

	UPROPERTY(EditAnywhere, Category = "InputParams")
	int32 TileWidth = 0;

	UPROPERTY(EditAnywhere, Category = "InputParams")
	int32 TileLength = 0;

	FVector CurrentActorLocation;

	class AObstacle* ObstacleTopBorder;
	class AObstacle* ObstacleBottomBorder;
	class AObstacle* ObstacleRightBorder;
	class AObstacle* ObstacleLeftBorder;

public:
	void ClearTileList();

	UFUNCTION(BlueprintCallable, Category = "Map")
	void GenerateMap(int32 CountWidthDir,  int32 CountLengthDir);
	ATileBase* GetFirstTile();
	ATileBase* GetLastTile(); 
	
	int32 GetMuchMoreOccupiedPlayerIndex();	
	void SpawnItemOnTile();
	void SpawnItemOnTargetTile(int32 TileIndex);
	void DestroySpawnedItemOnTile();
	void ConsumeItemOnTargetTile(int32 TileIndex);
	
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/TileGenerator.h"
#include "TileBase.h"
#include "Engine/GameViewportClient.h"

// Sets default values
ATileGenerator::ATileGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;	
	CurrentActorLocation = GetActorLocation();	
}

// Called when the game starts or when spawned
void ATileGenerator::BeginPlay()
{
	Super::BeginPlay();	
	TileCountWidth = 10; 
	TileCountLength = 10; 

	GenerateMap(TileCountWidth, TileCountLength);
}

void ATileGenerator::GenerateMap(int32 CountWidthDir, int32 CountLengthDir)
{	
	UE_LOG(LogTemp, Warning, TEXT("Current Location : %f, %f, %f"), CurrentActorLocation.X, CurrentActorLocation.Y, CurrentActorLocation.Z);
	int32 TotalCount = CountWidthDir * CountLengthDir;	
	ArrayOfTileRow.SetNum(CountLengthDir);
	float InitPosX = (0 - CountWidthDir * TileWidth) * 0.5f;
	float InitPosY = (0 - CountLengthDir * TileLength) * 0.5f;
	for (int32 i = 0; i < ArrayOfTileRow.Num(); i++)
	{
		TArray<ATileBase*> TempArray;
		TempArray.SetNum(CountLengthDir);
		for (int j = 0; j < TempArray.Num(); j++)
		{			
			FVector NewPosition = FVector(i * TileWidth + InitPosX, j * TileLength + InitPosY,  CurrentActorLocation.Z) + CurrentActorLocation;
			ATileBase* GenTile = GetWorld()->SpawnActor<ATileBase>(TileBaseClass, NewPosition, GetActorRotation());
			TempArray[j] = GenTile;			
		}
		
		ArrayOfTileRow[i] = TempArray;
	}	
}



void ATileGenerator::ClearTileList()
{
	//for (int32 i = 0; i < ArrayTileBase.Num(); i++)
	//{		
	//	if (ArrayTileBase[i])
	//	{
	//		ArrayTileBase[i]->Destroy();
	//	}
	//		
	//}

	//ArrayTileBase.Reset(0);
}




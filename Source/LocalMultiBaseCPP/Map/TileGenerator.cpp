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

	GenerateMap(TileCountWidth, TileCountLength);
}

void ATileGenerator::GenerateMap(int32 CountWidthDir, int32 CountLengthDir)
{	
	UE_LOG(LogTemp, Warning, TEXT("Current Location : %f, %f, %f"), CurrentActorLocation.X, CurrentActorLocation.Y, CurrentActorLocation.Z);
	
	ArrayOfTileRow.SetNum(CountLengthDir);
	float InitPosX = (0 - (CountWidthDir - 1) * TileWidth);
	float InitPosY = (0 - (CountLengthDir - 1) * TileLength);
	for (int32 i = 0; i < ArrayOfTileRow.Num(); i++)
	{
		TArray<ATileBase*> TempArray;
		TempArray.SetNum(CountWidthDir);
		for (int j = 0; j < TempArray.Num(); j++)
		{	
			FVector NewPosition = FVector(j * TileWidth * 2 + InitPosX, i * TileLength * 2 + InitPosY, CurrentActorLocation.Z);// +CurrentActorLocation;
			ATileBase* GenTile = GetWorld()->SpawnActor<ATileBase>(TileBaseClass, NewPosition, GetActorRotation());
			
			TempArray[j] = GenTile;			
		}
		
		ArrayOfTileRow[i] = TempArray;
	}	
}

ATileBase* ATileGenerator::GetFirstTile()
{
	if (ArrayOfTileRow.Num() > 0 )
	{
		if (ArrayOfTileRow[0].Num() > 0)
		{
			return ArrayOfTileRow[0][0];
		}			
	}
	return nullptr;
}

ATileBase* ATileGenerator::GetLastTile()
{
	if (ArrayOfTileRow.Num() > 0)
	{
		int32 LastIndexOfArrayGroup = ArrayOfTileRow.Num() - 1;
		if (ArrayOfTileRow[LastIndexOfArrayGroup].Num() > 0)
		{			
			return	ArrayOfTileRow[LastIndexOfArrayGroup].Last();		
		}
	}
	return nullptr;
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




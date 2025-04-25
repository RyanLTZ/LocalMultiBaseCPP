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
	GenerateMap(50, 100);
}

void ATileGenerator::GenerateMap(int32 CountWidthDir, int32 CountLengthDir)
{	
	UE_LOG(LogTemp, Warning, TEXT("Current Location : %f, %f, %f"), CurrentActorLocation.X, CurrentActorLocation.Y, CurrentActorLocation.Z);
	int32 TotalCount = CountWidthDir * CountLengthDir;
	//ArrayTileBaseRow.SetNum(CountWidthDir);
	ArrayOfTileRow.SetNum(CountWidthDir);
	int32 TilePosOffsetX = 100.f;
	int32 TilePosOffsetY = 100.f;
		
	for (int32 i = 0; i < ArrayOfTileRow.Num(); i++)
	{
		TArray<ATileBase*> TempArray;
		TempArray.SetNum(CountLengthDir);
		for (int j = 0; j < TempArray.Num(); j++)
		{
			FVector NewPosition = FVector(j * TilePosOffsetX, i * TilePosOffsetY,  CurrentActorLocation.Z) + CurrentActorLocation;
			ATileBase* GenTile = GetWorld()->SpawnActor<ATileBase>(TileBaseClass, NewPosition, GetActorRotation());
			TempArray[j] = GenTile;
		}
		
		ArrayOfTileRow[i] = TempArray;
	}	
}

void ATileGenerator::SetTilePosition(const TArray<ATileBase*> InputArray)
{
}

FVector ATileGenerator::GetTilePosition(const int32 ArrIndex, const int32 TotalCountInWidth, const int32 TotalCountInLength)
{
	float TileLength = 50.f;
	float TileWidth = 50.f;
	//FVector NewLocation = FVector(ArrIndex * TileWidth, ArrIndex * TileLength, CurrentActorLocation.Z) + CurrentActorLocation;
	int32 RowIndex = ArrIndex / TotalCountInWidth;
	int32 ColumnnIndex = ArrIndex / TotalCountInLength; // +ArrIndex % TotalCountInLength;
	int32 IndexInRow = ArrIndex % TotalCountInWidth;
	int32 IndexInColumn = ArrIndex % TotalCountInLength;
	FVector NewLocation = FVector(IndexInRow * TileWidth * ColumnnIndex, IndexInColumn * TileLength * RowIndex, CurrentActorLocation.Z) + CurrentActorLocation;

	return NewLocation;
	//TotalWidthCount = 10 
	//0~9 : y 0, z 0
	//10 ~ 19 : x + 1, y + 1
	
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




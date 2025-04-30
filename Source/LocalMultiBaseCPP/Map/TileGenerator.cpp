// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/TileGenerator.h"
#include "TileBase.h"
#include "Engine/GameViewportClient.h"
#include "Obstacle.h"

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

	ObstacleLeftBorder = GetWorld()->SpawnActor<AObstacle>(ObstacleClass, FVector::ZeroVector, GetActorRotation());
	ObstacleLeftBorder->SetBorderSizeVerticalAxisByTileCount(CountWidthDir, FVector(TileWidth, TileLength, 50.f));
	ObstacleLeftBorder->SetActorLocation(FVector(0, InitPosY - TileLength * 2, 0));

	ObstacleRightBorder = GetWorld()->SpawnActor<AObstacle>(ObstacleClass, FVector::ZeroVector, GetActorRotation());
	ObstacleRightBorder->SetBorderSizeVerticalAxisByTileCount(CountWidthDir, FVector(TileWidth, TileLength, 50.f));
	ObstacleRightBorder->SetActorLocation(FVector(0, InitPosY + TileCountLength * TileLength * 2, 0));

	ObstacleTopBorder = GetWorld()->SpawnActor<AObstacle>(ObstacleClass, FVector::ZeroVector, GetActorRotation());
	ObstacleTopBorder->SetBorderSizeHorizontalAxisByTileCount(CountLengthDir, FVector(TileWidth, TileLength, 50.f));
	ObstacleTopBorder->SetActorLocation(FVector(InitPosX + TileCountWidth * TileWidth * 2, 0, 0));

	ObstacleBottomBorder = GetWorld()->SpawnActor<AObstacle>(ObstacleClass, FVector::ZeroVector, GetActorRotation());
	ObstacleBottomBorder->SetBorderSizeHorizontalAxisByTileCount(CountLengthDir, FVector(TileWidth, TileLength, 50.f));
	ObstacleBottomBorder->SetActorLocation(FVector(InitPosX - TileWidth * 2, 0, 0));

	for (int32 i = 0; i < ArrayOfTileRow.Num(); i++)
	{
		TArray<ATileBase*> TempArray;
		TempArray.SetNum(CountWidthDir);
		for (int j = 0; j < TempArray.Num(); j++)
		{	
			FVector NewPosition = FVector(j * TileWidth * 2 + InitPosX, i * TileLength * 2 + InitPosY, CurrentActorLocation.Z);// +CurrentActorLocation;
			bool bObstacle = FMath::RandRange(0, 10) > 9;
			if (bObstacle && i > 0 && i < ArrayOfTileRow.Num() - 1)
			{
				AObstacle* GenObstacle = GetWorld()->SpawnActor<AObstacle>(ObstacleClass, NewPosition, GetActorRotation());
				TempArray[j] = GenObstacle;
			}
			else
			{
				ATileBase* GenTile = GetWorld()->SpawnActor<ATileBase>(TileBaseClass, NewPosition, GetActorRotation());
				TempArray[j] = GenTile;

			}
			
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

int32 ATileGenerator::GetMuchMoreOccupiedPlayerIndex()
{
	int32 Player0Count = 0, Player1Count = 0;
	
	for (int32 i = 0; i < ArrayOfTileRow.Num(); i++)
	{
		for (int32 j = 0; j < ArrayOfTileRow[i].Num(); j++)
		{
			if (ArrayOfTileRow[i][j]->GetOccupiedPlayerIndex() == 0)
			{
				Player0Count++;
			}
			else if (ArrayOfTileRow[i][j]->GetOccupiedPlayerIndex() == 1)
			{
				Player1Count++;
			}
		}
	}

	return Player0Count > Player1Count ? 0 : Player0Count == Player1Count ? -1 : 1;
}



void ATileGenerator::ClearTileList()
{
	for (int32 i = 0; i < ArrayOfTileRow.Num(); i++)
	{
		for (int32 j = 0; j < ArrayOfTileRow[i].Num(); j++)
		{
			ArrayOfTileRow[i][j]->Destroy();
		}	
	}

	ArrayOfTileRow.Reset(0);
}




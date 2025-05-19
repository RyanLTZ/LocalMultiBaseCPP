// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/TileGenerator.h"
#include "TileBase.h"
#include "Engine/GameViewportClient.h"
#include "Obstacle.h"
#include "Map/SpawItemBase.h"
#include "Map/DestructableObst.h"
#include "Map/RandomSpawnObject.h"

// Sets default values
ATileGenerator::ATileGenerator()
{
 	// Set this actor to call Tick() every frame.  You can tuudrn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;	
	CurrentActorLocation = GetActorLocation();	

	static ConstructorHelpers::FClassFinder<ASpawItemBase> BP_SpawnItemClass(TEXT("'/Game/Blueprints/BP_SpawnItemBase.BP_SpawnItemBase_C'"));
	if (BP_SpawnItemClass.Succeeded())
	{
		SpawnItemBaseClass = BP_SpawnItemClass.Class;
	}

	static ConstructorHelpers::FClassFinder<ADestructableObst> BP_DestructableObs(TEXT("'/Game/Blueprints/BP_DestructableObs.BP_DestructableObs_C'"));
	if (BP_DestructableObs.Succeeded())
	{
		DestructableObsClass = BP_DestructableObs.Class;
	}

	static ConstructorHelpers::FClassFinder<ARandomSpawnObject> BP_RandomSpawnObjectObj(TEXT("'/Game/Blueprints/BP_RandomSpawnObject.BP_RandomSpawnObject_C'"));
	if (BP_RandomSpawnObjectObj.Succeeded())
	{
		RandomSpawnObjectClass = BP_RandomSpawnObjectObj.Class;
	}	

	static ConstructorHelpers::FClassFinder<ARandomSpawnObject> BP_RandomObjType2Obj(TEXT("'/Game/Blueprints/BP_RandomSpawnObjectType2.BP_RandomSpawnObjectType2_C'"));
	if (BP_RandomSpawnObjectObj.Succeeded())
	{
		RandomSpawnObjectType2Class = BP_RandomObjType2Obj.Class;
	}

	static ConstructorHelpers::FClassFinder<ARandomSpawnObject> BP_RandomObjType4Obj(TEXT("'/Game/Blueprints/BP_RandomSpawnObjectType4.BP_RandomSpawnObjectType4_C'"));
	if (BP_RandomObjType4Obj.Succeeded())
	{
		RandomSpawnObjectType4Class = BP_RandomObjType4Obj.Class;
	}	

	static ConstructorHelpers::FClassFinder<AObstacle> BP_BorderObstacle(TEXT("'/Game/Blueprints/BP_BorderObstacle.BP_BorderObstacle_C'"));
	if (BP_BorderObstacle.Succeeded())
	{
		ObstacleType2Class = BP_BorderObstacle.Class;
	}
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
		
	float InitPosX = (0 - (CountWidthDir - 1) * TileWidth);
	float InitPosY = (0 - (CountLengthDir - 1) * TileLength);

	ObstacleLeftBorder = GetWorld()->SpawnActor<AObstacle>(ObstacleType2Class, FVector::ZeroVector, GetActorRotation());
	ObstacleLeftBorder->SetBorderSizeVerticalAxisByTileCount(CountWidthDir, FVector(TileWidth, TileLength, 100.f));
	ObstacleLeftBorder->SetActorLocation(FVector(0, InitPosY - TileLength * 2, 0));

	ObstacleRightBorder = GetWorld()->SpawnActor<AObstacle>(ObstacleType2Class, FVector::ZeroVector, GetActorRotation());
	ObstacleRightBorder->SetBorderSizeVerticalAxisByTileCount(CountWidthDir, FVector(TileWidth, TileLength, 100.f));
	ObstacleRightBorder->SetActorLocation(FVector(0, InitPosY + TileCountLength * TileLength * 2, 0));

	ObstacleTopBorder = GetWorld()->SpawnActor<AObstacle>(ObstacleType2Class, FVector::ZeroVector, GetActorRotation());
	ObstacleTopBorder->SetBorderSizeHorizontalAxisByTileCount(CountLengthDir, FVector(TileWidth, TileLength, 100.f));
	ObstacleTopBorder->SetActorLocation(FVector(InitPosX + TileCountWidth * TileWidth * 2, 0, 0));

	ObstacleBottomBorder = GetWorld()->SpawnActor<AObstacle>(ObstacleType2Class, FVector::ZeroVector, GetActorRotation());
	ObstacleBottomBorder->SetBorderSizeHorizontalAxisByTileCount(CountLengthDir, FVector(TileWidth, TileLength, 100.f));
	ObstacleBottomBorder->SetActorLocation(FVector(InitPosX - TileWidth * 2, 0, 0));
	
	int32 TileIndex = 0;
	FMath::SRandInit(2455);
	for (int32 i = 0; i < CountLengthDir; i++)
	{		
		for (int j = 0; j < CountWidthDir; j++)
		{	
			FVector NewPosition = FVector(j * TileWidth * 2 + InitPosX, i * TileLength * 2 + InitPosY, CurrentActorLocation.Z);// +CurrentActorLocation;						
			bool bDeployObstacles = FMath::RandRange(0, 100) > 90;						
			if (bDeployObstacles && i > 0 && i < CountLengthDir - 1)
			{
				//CreateAndDeployObstacles(TileIndex, NewPosition);
				int32 RandomRotationFactor = FMath::RandRange(-180, 180);
				if (FMath::RandRange(0, 100) > 70) //Destructable Obstacle
				{
					ADestructableObst* GenObstacle = GetWorld()->SpawnActor<ADestructableObst>(DestructableObsClass, NewPosition + FVector(0, 0, 50), GetActorRotation());
					GenObstacle->SetIndex(TileIndex);
				}
				else //Normal Obstacle
				{

					if (FMath::RandRange(0, 10) > 5)
					{						
						int32 Random = FMath::RandRange(0, 10);
						if (Random <= 3)
						{
							ARandomSpawnObject* GenRandomObj = GetWorld()->SpawnActor<ARandomSpawnObject>(RandomSpawnObjectType2Class, NewPosition + FVector(0, 0, 50), GetActorRotation() + FRotator(0, 90 + RandomRotationFactor, 0));
						}
						else if (Random <= 6)
						{
							ARandomSpawnObject* GenRandomObj = GetWorld()->SpawnActor<ARandomSpawnObject>(RandomSpawnObjectClass, NewPosition + FVector(0, 0, 50), GetActorRotation() + FRotator(0, 90 + RandomRotationFactor, 0));
						}
						else
						{
							ARandomSpawnObject* GenRandomObj = GetWorld()->SpawnActor<ARandomSpawnObject>(RandomSpawnObjectType4Class, NewPosition + FVector(0, 0, 50), GetActorRotation() + FRotator(0, 90 + RandomRotationFactor, 0));
						}

					}
					else
					{
						AObstacle* GenObstacle = GetWorld()->SpawnActor<AObstacle>(ObstacleClass, NewPosition + FVector(0, 0, 50), GetActorRotation());
						GenObstacle->SetIndex(TileIndex);
						if (FMath::RandRange(0, 10) > 7)
						{
							GenObstacle->SetTransitable();
						}
					}

				}
			}

			ATileBase* GenTile = GetWorld()->SpawnActor<ATileBase>(TileBaseClass, NewPosition, GetActorRotation());
			//TempArray[j] = GenTile;			
			GenTile->SetIndex(TileIndex);
			MapTileList.Add(TileIndex, GenTile);

			TileIndex++;
		}
		
		//ArrayOfTileRow[i] = TempArray;
	}	

}

void ATileGenerator::CreateAndDeployObstacles(int32 TileIndex, FVector NewPosition)
{
	int32 RandomDice = FMath::RandRange(0, 100);
	
	if (RandomDice > 70 && DestructableObstacleCount < MaxDestructableObstacleCount) //Destructable Obstacle
	{
		ADestructableObst* GenObstacle = GetWorld()->SpawnActor<ADestructableObst>(DestructableObsClass, NewPosition + FVector(0, 0, 50), GetActorRotation());
		GenObstacle->SetIndex(TileIndex);
		DestructableObstacleCount++;
	}
	else //Normal Obstacle
	{

		if (FMath::RandRange(0, 10) < 6)
		{
			int32 Random = FMath::RandRange(0, 10);
			if (Random >= 3 && RandomSpawnType2ObjCount < MaxRandomSpawnObjectCount)
			{
				ARandomSpawnObject* GenRandomObj = GetWorld()->SpawnActor<ARandomSpawnObject>(RandomSpawnObjectType2Class, NewPosition + FVector(0, 0, 50), GetActorRotation() + FRotator(0, 90, 0));
				RandomSpawnType2ObjCount++;
			}
			else if (Random >= 6 && RandomSpawnDefaultObjCount < MaxRandomSpawnObjectCount)
			{
				ARandomSpawnObject* GenRandomObj = GetWorld()->SpawnActor<ARandomSpawnObject>(RandomSpawnObjectClass, NewPosition + FVector(0, 0, 50), GetActorRotation() + FRotator(0, 90, 0));
				RandomSpawnDefaultObjCount++;
			}
			else 
			{
				if (RandomSpawnType4ObjCount < MaxRandomSpawnType4Count)
				{
					ARandomSpawnObject* GenRandomObj = GetWorld()->SpawnActor<ARandomSpawnObject>(RandomSpawnObjectType4Class, NewPosition + FVector(0, 0, 50), GetActorRotation() + FRotator(0, 90, 0));
					RandomSpawnType4ObjCount++;
				}
				
			}

		}
		else 
		{
			if (DefaultObstaceCount < MaxDefaultObstacleCount)
			{
				AObstacle* GenObstacle = GetWorld()->SpawnActor<AObstacle>(ObstacleClass, NewPosition + FVector(0, 0, 50), GetActorRotation());
				GenObstacle->SetIndex(TileIndex);
				if (FMath::RandRange(0, 10) > 7)
				{
					GenObstacle->SetTransitable();
				}

				DefaultObstaceCount++;
			}
		}

	}
}

ATileBase* ATileGenerator::GetFirstTile()
{
	if (MapTileList.Num() > 0 )
	{
		return MapTileList[0];
	}
	return nullptr;
}

ATileBase* ATileGenerator::GetLastTile()
{
	if (MapTileList.Num() > 0)
	{
		return MapTileList[MapTileList.Num() - 1];
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

void ATileGenerator::SpawnItemOnTile()
{
	ArrayItem.SetNum(5);
	for (int32 i = 0; i < 5; i++)
	{
		int32 TargetTileIndex = FMath::RandRange(0, MapTileList.Num() - 1);
		ATileBase* TargetTile = MapTileList[TargetTileIndex];
		if (TargetTile)
		{
			ASpawItemBase* SpawnItem = GetWorld()->SpawnActor<ASpawItemBase>(SpawnItemBaseClass, TargetTile->GetActorLocation(), FRotator::ZeroRotator);
			ArrayItem.Add(SpawnItem);
		}
	}	

}

void ATileGenerator::SpawnItemOnTargetTile(int32 TileIndex)
{
	ATileBase* TargetTile = MapTileList[TileIndex];
	if (TargetTile)
	{
		ASpawItemBase* SpawnItem = GetWorld()->SpawnActor<ASpawItemBase>(SpawnItemBaseClass, TargetTile->GetActorLocation(), FRotator::ZeroRotator);
		ArrayItem.Add(SpawnItem);
	}
}

void ATileGenerator::DestroySpawnedItemOnTile()
{	
	UE_LOG(LogTemp, Warning, TEXT("Item Destroy"));
	if (ArrayItem.Num() > 0)
	{
		for (int32 i = 0; i < ArrayItem.Num(); i++)
		{
			if (ArrayItem[i])
			{
				ArrayItem[i]->Destroy();							
			}				
		}
	}
}

void ATileGenerator::ConsumeItemOnTargetTile(int32 TileIndex)
{
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
	MapTileList.Reset();
}




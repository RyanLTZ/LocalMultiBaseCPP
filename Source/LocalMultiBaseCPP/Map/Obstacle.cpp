// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/Obstacle.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

AObstacle::AObstacle()
{
}

void AObstacle::SetBorderSizeHorizontalAxisByTileCount(int32 Count, const FVector TileSize)
{
	if (BoxComponent)
	{
		FVector NewExtent = FVector::ZeroVector;
		NewExtent.Set(TileSize.X, TileSize.Y * Count, 100.f);
		BoxComponent->SetBoxExtent(NewExtent);
		
		MeshComponent->SetWorldScale3D(FVector(1, Count, 1));		
	}
}

void AObstacle::SetBorderSizeVerticalAxisByTileCount(int32 Count, const FVector TileSize)
{
	FVector NewExtent = FVector::ZeroVector;
	NewExtent.Set(TileSize.X * Count, TileSize.Y, 100.f);
	BoxComponent->SetBoxExtent(NewExtent);

	MeshComponent->SetWorldScale3D(FVector(Count, 1, 1));

}

void AObstacle::SetTransitable()
{
	BoxComponent->SetGenerateOverlapEvents(false);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//BoxComponent->SetCollisionObjectType(ECC_GameTraceChannel1);
	//BoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	//BoxComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap);
	//BoxComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
}




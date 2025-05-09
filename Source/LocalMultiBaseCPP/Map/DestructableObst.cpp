// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/DestructableObst.h"
#include "Map/SpawItemBase.h"
#include "Game/LMGameModeBase.h"
#include "TileBase.h"

void ADestructableObst::BeginPlay()
{
	Super::BeginPlay();
}

void ADestructableObst::SpawnOther()
{
	AGameModeBase* CurrentMode = GetWorld()->GetAuthGameMode();
	ALMGameModeBase* GameMode = Cast<ALMGameModeBase>(CurrentMode);
	if (GameMode)
	{			
		GameMode->OnDestructableObstacleDestroyed(TileIndex);
	}
}

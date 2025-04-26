// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/LMGameModeBase.h"
#include "Pawn/LMPawnPlayer.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "GameFrameWork/PlayerStart.h"
#include "Player/LMPlayerController.h"
#include "Map/TileGenerator.h"

ALMGameModeBase::ALMGameModeBase()
{
	DefaultPawnClass = nullptr; // ALMPawnPlayer::StaticClass();
	LMPawnBaseClass = ALMPawnPlayer::StaticClass();			
	PlayerControllerClass = ALMPlayerController::StaticClass();		
}

void ALMGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	const FName TargetTag1P = TEXT("PlayerStart1P");
	const FName TargetTag2P = TEXT("PlayerStart2P");
	
	PlayerStart1P = FindPlayerStart(World, TargetTag1P);
	PlayerStart2P = FindPlayerStart(World, TargetTag2P);

	SpawnLocalPlayer(0, PlayerStart1P, World);	
	SpawnLocalPlayer(1, PlayerStart2P, World);
}

APlayerStart* ALMGameModeBase::FindPlayerStart(UWorld* World, const FName& TargetTag)
{
	if (World)
	{
		for (TActorIterator<APlayerStart> It(World); It; ++It)
		{
			APlayerStart* FoundStart = *It;
			if (FoundStart && FoundStart->PlayerStartTag.IsEqual(TargetTag))
			{
				return FoundStart;
			}
		}
	}
	return nullptr;
}

void ALMGameModeBase::SpawnLocalPlayer(int32 PlayerIndex, APlayerStart* PlayerStart, UWorld* World)
{
	if (PlayerStart == nullptr || World == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Null on PlayerStart or World"), PlayerIndex);
			
		return;
	}
	
	if (PlayerIndex == 0)
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, PlayerIndex);
		SpawnAndPossessPawn(World, PlayerController, PlayerStart, PlayerIndex);
	}
	else if (PlayerIndex == 1)
	{
		UGameInstance* GameInstance = GetGameInstance();
		ensure(GameInstance);
		FString Error;
		ULocalPlayer* NewLocalPlayer = GameInstance->CreateLocalPlayer(-1, Error, true);
		ensure(NewLocalPlayer);
		APlayerController* PlayerController2P = NewLocalPlayer->GetPlayerController(World);				
		SpawnAndPossessPawn(World, PlayerController2P, PlayerStart, PlayerIndex);		
	}

}

ALMPawnPlayer* ALMGameModeBase::SpawnAndPossessPawn(UWorld* World, APlayerController* PlayerController, APlayerStart* PlayerStart, int32 PlayerIndex)
{
	if (!World || !PlayerController || !PlayerStart)
	{
		UE_LOG(LogTemp, Error, TEXT("Null on PlayerStart or World"), PlayerIndex);
		return nullptr;
	}

	ALMPawnPlayer* PlayerPawn = World->SpawnActor<ALMPawnPlayer>(LMPawnBaseClass, PlayerStart->GetActorLocation(), PlayerStart->GetActorRotation());
	ensure(PlayerPawn);
	PlayerPawn->SetPlayerIndex(PlayerIndex);
	PlayerController->Possess(PlayerPawn);	
	return PlayerPawn;
}

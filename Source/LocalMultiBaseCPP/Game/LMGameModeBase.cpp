// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/LMGameModeBase.h"
#include "Pawn/LMPawnPlayer.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "GameFrameWork/PlayerStart.h"
#include "Player/LMPlayerController.h"
#include "Map/TileGenerator.h"
#include "Map/TileBase.h"
#include "Game/GameManager.h"
#include "Wigets/MainHUDWidget.h"
#include "Blueprint/UserWidget.h"

ALMGameModeBase::ALMGameModeBase()
{
	DefaultPawnClass = nullptr; // ALMPawnPlayer::StaticClass();
	
	static ConstructorHelpers::FClassFinder<ALMPawnPlayer> BP_LMPawnPlayer(TEXT("'/Game/Blueprints/BP_LMPawnPlayer.BP_LMPawnPlayer_C'"));
	if (BP_LMPawnPlayer.Succeeded())
	{
		LMPawnPlayerClass = BP_LMPawnPlayer.Class;
	}

	PlayerControllerClass = ALMPlayerController::StaticClass();	

	static ConstructorHelpers::FClassFinder<ATileGenerator> BP_LMTileGenerator(TEXT("'/Game/Blueprints/BP_TileGenerator.BP_TileGenerator_C'"));
	if (BP_LMTileGenerator.Succeeded())
	{
		TileGeneratorClass = BP_LMTileGenerator.Class;
	}

	GameManagerClass = AGameManager::StaticClass();

	static ConstructorHelpers::FClassFinder<UMainHUDWidget> BP_MainHUD(TEXT("'/Game/Widget/BP_MainHudWidget.BP_MainHudWidget_C'"));
	if (BP_MainHUD.Succeeded())
	{
		MainHUDWidgetClass = BP_MainHUD.Class;
	}	
}

void ALMGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();

	if (LMPawnPlayerClass == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("LMPawnPlayerClass is nullptr!"));
			
		return;
	}	

	TileGenerator = GetWorld()->SpawnActor<ATileGenerator>(TileGeneratorClass, FVector::ZeroVector, FRotator::ZeroRotator);
	if (TileGenerator)
	{
		SpawnLocalPlayer(0, TileGenerator->GetFirstTile(), World);
		SpawnLocalPlayer(1, TileGenerator->GetLastTile(), World);
	}		
	

	if (MainHUDWidgetClass)
	{
		MainHUD = CreateWidget<UMainHUDWidget>(GetWorld(), MainHUDWidgetClass);
	}
	

	if (MainHUD)
	{
		MainHUD->AddToViewport();				
	}

	GameManager = GetWorld()->SpawnActor<AGameManager>(GameManagerClass, FVector::ZeroVector, FRotator::ZeroRotator);
	if (GameManager)
	{
		GameManager->FUNCDeleOnGameFinish.BindUFunction(this, FName("OnGameFinished"));
		GameManager->FUNCDeleOnTimeChange.BindUFunction(this, FName("OnTimeChange"));
		GameManager->SetRemainTime(30.f);
		GameManager->FUNCDeleOnItemDestroy.BindUFunction(this, FName("OnDeleItemDestroy"));
		GameManager->FUNCDeleOnItemSpawn.BindUFunction(this, FName("OnDeleItemSpawn"));
	}
	
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

void ALMGameModeBase::SpawnLocalPlayer(int32 PlayerIndex, ATileBase* StartTile, UWorld* World)
{
	if (StartTile == nullptr || World == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Null on PlayerStart or World"), PlayerIndex);

		return;
	}

	if (PlayerIndex == 0)
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, PlayerIndex);
		SpawnAndPossessPawn(World, PlayerController, StartTile, PlayerIndex);
	}
	else if (PlayerIndex == 1)
	{
		UGameInstance* GameInstance = GetGameInstance();
		ensure(GameInstance);
		FString Error;
		ULocalPlayer* NewLocalPlayer = GameInstance->CreateLocalPlayer(-1, Error, true);
		ensure(NewLocalPlayer);
		APlayerController* PlayerController2P = NewLocalPlayer->GetPlayerController(World);
		SpawnAndPossessPawn(World, PlayerController2P, StartTile, PlayerIndex);
	}
}

ALMPawnPlayer* ALMGameModeBase::SpawnAndPossessPawn(UWorld* World, APlayerController* PlayerController, APlayerStart* PlayerStart, int32 PlayerIndex)
{
	if (!World || !PlayerController || !PlayerStart)
	{
		UE_LOG(LogTemp, Error, TEXT("Null on PlayerStart or World"), PlayerIndex);
		return nullptr;
	}

	ALMPawnPlayer* PlayerPawn = World->SpawnActor<ALMPawnPlayer>(LMPawnPlayerClass, PlayerStart->GetActorLocation(),FRotator::ZeroRotator);
	ensure(PlayerPawn);
	PlayerPawn->SetPlayerIndex(PlayerIndex);
	PlayerController->Possess(PlayerPawn);	
	return PlayerPawn;
}

ALMPawnPlayer* ALMGameModeBase::SpawnAndPossessPawn(UWorld* World, APlayerController* PlayerController, ATileBase* PlayerStart, int32 PlayerIndex)
{
	if (!World || !PlayerController || !PlayerStart)
	{
		UE_LOG(LogTemp, Error, TEXT("Null on PlayerStart or World"), PlayerIndex);
		return nullptr;
	}
	
	FVector NewLocation = PlayerStart->GetActorLocation() + FVector(0, 0, 60);
	ALMPawnPlayer* PlayerPawn = World->SpawnActor<ALMPawnPlayer>(LMPawnPlayerClass, NewLocation, PlayerStart->GetActorRotation());
	ensure(PlayerPawn);
	PlayerPawn->SetPlayerIndex(PlayerIndex);		
	PlayerController->Possess(PlayerPawn);
	return PlayerPawn;
}

void ALMGameModeBase::OnGameFinished()
{
	int32 WinPlayerIndex = TileGenerator->GetMuchMoreOccupiedPlayerIndex();	
	if (WinPlayerIndex < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Tie Game"));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Player %d is Winner"), WinPlayerIndex + 1);
	
}

void ALMGameModeBase::OnSubScore(int32 PlayerIndex)
{
	if (PlayerIndex == 0)
	{
		Player0Score--;
		Player0Score = FMath::Max(0, Player0Score);	
	}
	else
	{
		Player1Score--;
		Player1Score = FMath::Max(0, Player1Score);
	}

	MainHUD->UpdateSocre(0, Player0Score);
	MainHUD->UpdateSocre(1, Player1Score);
}

void ALMGameModeBase::OnDestructableObstacleDestroyed(int32 TileIndex)
{
	TileGenerator->SpawnItemOnTargetTile(TileIndex);
}

void ALMGameModeBase::OnTimeChange(float Time)
{
	MainHUD->UpdateTimer(FMath::Abs(Time));
}

void ALMGameModeBase::OnDeleItemDestroy()
{
	if (TileGenerator)
	{
		TileGenerator->DestroySpawnedItemOnTile();
	}
}

void ALMGameModeBase::OnDeleItemSpawn()
{
	if (TileGenerator)
	{
		TileGenerator->SpawnItemOnTile();
	}
}

void ALMGameModeBase::OnAddScore(int32 PlayerIndex)
{
	if (PlayerIndex == 0)
	{
		Player0Score++;		
	}
	else
	{
		Player1Score++;		
	}

	MainHUD->UpdateSocre(0, Player0Score);
	MainHUD->UpdateSocre(1, Player1Score);
}

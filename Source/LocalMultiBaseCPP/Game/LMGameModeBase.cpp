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
#include "Player/SkillLightningAttack.h"
#include "Player/SkillStunAttack.h"

ALMGameModeBase::ALMGameModeBase()
{
	DefaultPawnClass = nullptr; // ALMPawnPlayer::StaticClass();

	ConstructorHelpers::FClassFinder<ALMPawnPlayer> BP_LMPawnPlayer(TEXT("'/Game/Blueprints/BP_LMPawnPlayer.BP_LMPawnPlayer_C'"));

	if (BP_LMPawnPlayer.Succeeded())
	{
		// 3. LMPawnPlayerClass 도 세팅하고, DefaultPawnClass 도 세팅
		LMPawnPlayerClass = BP_LMPawnPlayer.Class;
		//DefaultPawnClass = BP_LMPawnPlayer.Class;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("BP_LMPawnPlayer을 찾지 못했습니다! 경로를 확인하세요."));
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

	static ConstructorHelpers::FClassFinder<ASkillLightningAttack> BP_LightningAtk(TEXT("'/Game/Blueprints/Skill/BP_LightingAttack.BP_LightingAttack_C'"));
	if (BP_LightningAtk.Succeeded())
	{
		LightningAtkClass = BP_LightningAtk.Class;
	}

	static ConstructorHelpers::FClassFinder<ASkillStunAttack> BP_StunAtk(TEXT("'/Game/Blueprints/Skill/BP_StunAttack.BP_StunAttack_C'"));
	if (BP_StunAtk.Succeeded())
	{
		StunAttackClass = BP_StunAtk.Class;
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
		GameManager->FUNCDeleOnItemDestroy.BindUFunction(this, FName("OnDeleItemDestroy"));
		GameManager->FUNCDeleOnItemSpawn.BindUFunction(this, FName("OnDeleItemSpawn"));
		GameManager->SetRemainTime(GameDuration);
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
		PawnPlayer1 = SpawnAndPossessPawn(World, PlayerController, StartTile, PlayerIndex);
	}
	else if (PlayerIndex == 1)
	{
		UGameInstance* GameInstance = GetGameInstance();
		ensure(GameInstance);
		FString Error;

		ULocalPlayer* SecondLocalPlayer = GameInstance->GetLocalPlayerByIndex(1);

		if (SecondLocalPlayer)
		{
			APlayerController* PlayerController2P = SecondLocalPlayer->GetPlayerController(World);
			PawnPlayer2 = SpawnAndPossessPawn(World, PlayerController2P, StartTile, PlayerIndex);
			return;
		}

		SecondLocalPlayer = GameInstance->CreateLocalPlayer(-1, Error, true);
		ensure(SecondLocalPlayer);
		APlayerController* PlayerController2P = SecondLocalPlayer->GetPlayerController(World);
		PawnPlayer2 = SpawnAndPossessPawn(World, PlayerController2P, StartTile, PlayerIndex);

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

	AdditionalEvent_OnGameEnded();

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

void ALMGameModeBase::OnConsumeItem(ASpawItemBase* TargetItem)
{
}

void ALMGameModeBase::OnPlayerDead(int32 TargetIdx, int32 KillerIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("TargetIdx : %d"), TargetIdx);

	if (TargetIdx == 0 && PawnPlayer1)
	{
		if (KillerIndex == 1)
		{
			StatusP1.KillCount++;
		}

		PawnPlayer1->Destroy();
				
	}
	else if (TargetIdx == 1 && PawnPlayer2)
	{
		if (KillerIndex == 0)
		{
			StatusP2.KillCount++;
		}

		PawnPlayer2->Destroy();
	
	}

	Event_OnPlayerDied(TargetIdx);
	SpawnPlayer(TargetIdx);
}

void ALMGameModeBase::SpawnPlayer(int32 TargetIdx)
{	
	if (TileGenerator)
	{
		if (TargetIdx == 0)
		{
			StatusP1.RespawnCount++;
			SpawnLocalPlayer(TargetIdx, TileGenerator->GetFirstTile(), GetWorld());
		}
		else if (TargetIdx == 1)
		{
			StatusP2.RespawnCount++;
			SpawnLocalPlayer(TargetIdx, TileGenerator->GetLastTile(), GetWorld());
		}
		
	}
}

void ALMGameModeBase::DoLightningAttack(int32 OwnerIndex)
{
	FActorSpawnParameters SpawnParams;
	ALMPawnPlayer* TargetPlayer;
	OwnerIndex == 0 ? TargetPlayer = PawnPlayer2 : TargetPlayer = PawnPlayer1;
	
	ASkillLightningAttack* LightingAtk = Cast<ASkillLightningAttack>(UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), LightningAtkClass, TargetPlayer->GetActorTransform()));
	if (LightingAtk)
	{
		LightingAtk->SetOnwerPlayer(OwnerIndex);		
		UE_LOG(LogTemp, Warning, TEXT("TargetIndex %d, Owner Index %d"), TargetPlayer->GetPlayerIndex(), OwnerIndex);
		UGameplayStatics::FinishSpawningActor(LightingAtk, TargetPlayer->GetActorTransform());
	}	
}

void ALMGameModeBase::DoStunAttack(int32 OwnerIndex)
{
	FActorSpawnParameters SpawnParams;
	ALMPawnPlayer* TargetPlayer;
	OwnerIndex == 0 ? TargetPlayer = PawnPlayer2 : TargetPlayer = PawnPlayer1;

	ASkillStunAttack* StunAtk = Cast<ASkillStunAttack>(UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), StunAttackClass, TargetPlayer->GetActorTransform()));
	if (StunAtk)
	{
		StunAtk->SetOnwerPlayer(OwnerIndex);
		UE_LOG(LogTemp, Warning, TEXT("TargetIndex %d, Owner Index %d"), TargetPlayer->GetPlayerIndex(), OwnerIndex);
		UGameplayStatics::FinishSpawningActor(StunAtk, TargetPlayer->GetActorTransform());
		FBuffDebuffData Data;
		Data.Init();		
		TargetPlayer->SetDebuff(ELMDebuffType::Stun, Data);			
	}
}

void ALMGameModeBase::Event_OnPlayerDied_Implementation(int inputIndex)
{
}

void ALMGameModeBase::AdditionalEvent_OnGameEnded_Implementation()
{
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

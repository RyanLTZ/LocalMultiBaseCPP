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

}

UClass* ALMGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{

	// --- Pawn BP ---
	static const FSoftClassPath PawnRef(TEXT("/Game/Blueprints/BP_LMPawnPlayer.BP_LMPawnPlayer_C"));
	UClass* PawnBP = PawnRef.TryLoadClass<APawn>();
	if (PawnBP)
	{
		LMPawnPlayerClass = PawnBP;
		DefaultPawnClass = PawnBP;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("BP_LMPawnPlayer 로드 실패: %s"), *PawnRef.ToString());
	}

	// --- PlayerController BP ---
	static const FSoftClassPath PCRef(TEXT("/Game/Blueprints/BP_LMPlayerController.BP_LMPlayerController_C"));
	if (UClass* PCBP = PCRef.TryLoadClass<APlayerController>())
	{
		PlayerControllerClass = PCBP;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("BP_LMPlayerController 로드 실패: %s"), *PCRef.ToString());
	}

	// --- TileGenerator BP ---
	static const FSoftClassPath TileRef(TEXT("/Game/Blueprints/BP_TileGenerator.BP_TileGenerator_C"));
	if (UClass* TileBP = TileRef.TryLoadClass<ATileGenerator>())
	{
		TileGeneratorClass = TileBP;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("BP_TileGenerator 로드 실패: %s"), *TileRef.ToString());
	}

	// --- GameManager(C++) ---
	GameManagerClass = AGameManager::StaticClass();

	// --- MainHUDWidget BP ---
	static const FSoftClassPath HUDRef(TEXT("/Game/Widget/BP_MainHudWidget.BP_MainHudWidget_C"));
	if (UClass* HUDBP = HUDRef.TryLoadClass<UUserWidget>())
	{
		MainHUDWidgetClass = HUDBP;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("BP_MainHudWidget 로드 실패: %s"), *HUDRef.ToString());
	}

	// --- Skill: Lightning ---
	static const FSoftClassPath LightningRef(TEXT("/Game/Blueprints/Skill/BP_LightingAttack.BP_LightingAttack_C"));
	if (UClass* LightningBP = LightningRef.TryLoadClass<ASkillLightningAttack>())
	{
		LightningAtkClass = LightningBP;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("BP_LightingAttack 로드 실패: %s"), *LightningRef.ToString());
	}

	// --- Skill: Stun ---
	static const FSoftClassPath StunRef(TEXT("/Game/Blueprints/Skill/BP_StunAttack.BP_StunAttack_C"));
	if (UClass* StunBP = StunRef.TryLoadClass<ASkillStunAttack>())
	{
		StunAttackClass = StunBP;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("BP_StunAttack 로드 실패: %s"), *StunRef.ToString());
	}

	// --- 최종적으로 Pawn만 반환 ---
	return PawnBP
		? PawnBP
		: Super::GetDefaultPawnClassForController_Implementation(InController);
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
	int32 WinPlayerIndex = -1; 
	if (StatusP1.OccupiedTile > StatusP2.OccupiedTile)
	{
		WinPlayerIndex = 0; 
	}
	else if (StatusP1.OccupiedTile > StatusP2.OccupiedTile)
	{
		WinPlayerIndex = 1;
	}
	else
	{
		if (StatusP1.KillCount > StatusP2.KillCount)
		{
			WinPlayerIndex = 0;
		}
		else if (StatusP1.KillCount > StatusP2.KillCount)
		{
			WinPlayerIndex = 1; 
		}
	}		

	AdditionalEvent_OnGameEnded();
}

void ALMGameModeBase::OnSubScore(int32 PlayerIndex)
{
	if (PlayerIndex == 0)
	{
		StatusP1.OccupiedTile--;		
		StatusP1.OccupiedTile = FMath::Max(0, StatusP1.OccupiedTile);
	}
	else
	{
		StatusP2.OccupiedTile--;
		StatusP2.OccupiedTile = FMath::Max(0, StatusP2.OccupiedTile);
	}

	if (MainHUD)
	{
		MainHUD->UpdateSocre(0, StatusP1.OccupiedTile);
		MainHUD->UpdateSocre(1, StatusP2.OccupiedTile);
	}
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
	UE_LOG(LogTemp, Warning, TEXT("KillerIndex : %d"), KillerIndex);

	if (TargetIdx == 0 && PawnPlayer1) //1P died
	{
		if (KillerIndex == 1)
		{
			StatusP2.KillCount++;
		}

		PawnPlayer1->Destroy();
				
	}
	else if (TargetIdx == 1 && PawnPlayer2)  //2p died
	{
		if (KillerIndex == 0)
		{
			StatusP1.KillCount++;
		}

		PawnPlayer2->Destroy();
	
	}

	SpawnPlayer(TargetIdx);
	Event_OnPlayerDied(TargetIdx);
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
		UE_LOG(LogTemp, Warning, TEXT("Lightning Attack TargetIndex %d, Owner Index %d"), TargetPlayer->GetPlayerIndex(), OwnerIndex);
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
		UE_LOG(LogTemp, Warning, TEXT("Stun Attack TargetIndex %d, Owner Index %d"), TargetPlayer->GetPlayerIndex(), OwnerIndex);
		UGameplayStatics::FinishSpawningActor(StunAtk, TargetPlayer->GetActorTransform());
		UBuffDebuff* BuffData = NewObject<UBuffDebuff>();
		TargetPlayer->SetDebuff(ELMDebuffType::Stun, BuffData);				
	}
}

void ALMGameModeBase::Event_OnPlayerDied_Implementation(int inputIndex)
{
}

void ALMGameModeBase::AdditionalEvent_OnGameEnded_Implementation()
{
}

void ALMGameModeBase::ShowOrCreateMainHUD( bool bShow )
{
	if (bShow)
	{
		if (MainHUDWidgetClass)
		{
			if (MainHUD == nullptr)
			{
				MainHUD = CreateWidget<UMainHUDWidget>(GetWorld(), MainHUDWidgetClass);
			}

			if (MainHUD)
			{
				MainHUD->AddToViewport();
			}
		}

	}
	else
	{
		if (MainHUD)
		{
			MainHUD->RemoveFromViewport();
		}
	}

}


void ALMGameModeBase::OnTimeChange(float Time)
{
	if (MainHUD)
	{
		MainHUD->UpdateTimer(FMath::Abs(Time));
	}	
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
		StatusP1.OccupiedTile++;
	}
	else
	{
		StatusP2.OccupiedTile++;		
	}

	if (MainHUD)
	{
		MainHUD->UpdateSocre(0, StatusP1.OccupiedTile);
		MainHUD->UpdateSocre(1, StatusP2.OccupiedTile);
	}
}

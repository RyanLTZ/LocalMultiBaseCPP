// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LMGameModeBase.generated.h"

/**
 * 
 */



UCLASS()
class LOCALMULTIBASECPP_API ALMGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALMGameModeBase();

public:
	UPROPERTY(EditAnywhere, Category = "Player")
	//TSubclassOf<class ALMPawnBase> LMPawnBaseClass; 
	TSubclassOf<class ALMPawnPlayer> LMPawnPlayerClass;

	UPROPERTY(EditAnywhere, Category = "Player")
	class APlayerStart* PlayerStart1P; 

	UPROPERTY(EditAnywhere, Category = "Player")
	class APlayerStart* PlayerStart2P;

	UPROPERTY(EditAnywhere, Category = "Map")
	TSubclassOf<class ATileGenerator> TileGeneratorClass; 

	TSubclassOf<class AGameManager> GameManagerClass;	

	TSubclassOf<class UMainHUDWidget> MainHUDWidgetClass; 

protected:
	virtual void BeginPlay() override;

private:
	//Tag를 이용하여 PlayerStart를 찾는 함수
	APlayerStart* FindPlayerStart(UWorld* World, const FName& TargetTag);	

	//로컬 플레이어를 스폰하는 함수
	void SpawnLocalPlayer(int32 PlayerIndex, APlayerStart* PlayerStart, UWorld* World);

	void SpawnLocalPlayer(int32 PlayerIndex, class ATileBase* StartTile,  UWorld* World);

	class ALMPawnPlayer* SpawnAndPossessPawn(UWorld* World, APlayerController* PlayerController, APlayerStart* PlayerStart, int32 PlayerIndex);

	class ALMPawnPlayer* SpawnAndPossessPawn(UWorld* World, APlayerController* PlayerController, class ATileBase* PlayerStart, int32 PlayerIndex);

	

private:
	int32 SpawnedPlayerIndex = 0;
	class ATileGenerator* TileGenerator;
	class ALMPawnPlayer* PawnPlayer1; 
	class AGameManager* GameManager; 	
	class UMainHUDWidget* MainHUD;

	int32 Player0Score = 0;
	int32 Player1Score = 0; 
	
	UFUNCTION()
	void OnGameFinished();

public:
	UFUNCTION(BlueprintCallable)
	void OnAddScore(int32 PlayerIndex);
	
	void OnSubScore(int32 PlayerIndex);

};

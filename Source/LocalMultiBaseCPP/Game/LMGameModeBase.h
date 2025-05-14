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

	TSubclassOf<class ASkillLightningAttack> LightningAtkClass;

	TSubclassOf<class ASkillStunAttack> StunAttackClass; 

protected:
	virtual void BeginPlay() override;

private:
	//Tag를 이용하여 PlayerStart를 찾는 함수
	APlayerStart* FindPlayerStart(UWorld* World, const FName& TargetTag);	

	//로컬 플레이어를 스폰하는 함수
	void SpawnLocalPlayer(int32 PlayerIndex, class ATileBase* StartTile,  UWorld* World);

	class ALMPawnPlayer* SpawnAndPossessPawn(UWorld* World, APlayerController* PlayerController, APlayerStart* PlayerStart, int32 PlayerIndex);

	class ALMPawnPlayer* SpawnAndPossessPawn(UWorld* World, APlayerController* PlayerController, class ATileBase* PlayerStart, int32 PlayerIndex);	
	
	//GameRule
public:
	UPROPERTY(EditAnywhere, Category = "GameRule")
	float GameDuration = 240.f;

private:
	int32 SpawnedPlayerIndex = 0;
	class ATileGenerator* TileGenerator;
	class ALMPawnPlayer* PawnPlayer1; 
	class ALMPawnPlayer* PawnPlayer2;
	class AGameManager* GameManager; 
	
	UPROPERTY(BlueprintReadOnly, Category = "Score", meta = (AllowPrivateAccess = "true"))
	int32 Player0Score = 0;
	
	UPROPERTY(BlueprintReadOnly, Category = "Score", meta = (AllowPrivateAccess = "true"))
	int32 Player1Score = 0; 
	
	UFUNCTION()
	void OnGameFinished();

	UFUNCTION()
	void OnTimeChange(float Time);

	UFUNCTION()
	void OnDeleItemDestroy();
	
	UFUNCTION()
	void OnDeleItemSpawn();

public:	
	UFUNCTION(BlueprintCallable)
	void OnAddScore(int32 PlayerIndex);
	
	UFUNCTION(BlueprintCallable)
	void OnSubScore(int32 PlayerIndex);

	UFUNCTION(BlueprintCallable)
	void OnDestructableObstacleDestroyed(int32 TileIndex);	

	UFUNCTION(BlueprintCallable)
	void OnConsumeItem(class ASpawItemBase* TargetItem);

	UFUNCTION(BlueprintCallable)
	void OnPlayerDead(int32 TargetIdx);

	void SpawnPlayer(int32 TargetIdx);

	UFUNCTION(BlueprintCallable)
	void DoLightningAttack(int32 OwnerIndex);

	UFUNCTION(BlueprintCallable)
	void DoStunAttack(int32 OwnerIndex);

	FORCEINLINE
		ALMPawnPlayer* GetPlayerByIndex(int32 TargetIndex) { ALMPawnPlayer* ReturnValue;  TargetIndex == 0 ? ReturnValue = PawnPlayer1 : ReturnValue = PawnPlayer2; return ReturnValue; }
// 0514 한규 추가
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Item")
	void AdditionalEvent_OnGameEnded();
	virtual void AdditionalEvent_OnGameEnded_Implementation();


public :


	// 0512 한규 추가
  // BeginPlay 등에서 CreateWidget 으로 만든 인스턴스를 그대로 돌려주는 함수
	UFUNCTION(BlueprintPure, Category = "UI")
	UMainHUDWidget* GetMainHUD() const { return MainHUD; }

protected:
	// BeginPlay() 에서 실제로 생성한 MainHUD
	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	UMainHUDWidget* MainHUD;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UMainHUDWidget> MainHUDWidgetClass;
};

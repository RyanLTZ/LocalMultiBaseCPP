// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LMGameModeBase.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct LOCALMULTIBASECPP_API  FGameSatus
{
	GENERATED_BODY()

	/** 킬 수 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Status")
	int32 KillCount = 0;

	/** 점령 타일 수 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Status")
	int32 OccupiedTile = 0;

	/** 리스폰 횟수 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Status")
	int32 RespawnCount = 0;
};

UCLASS()
class LOCALMULTIBASECPP_API ALMGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALMGameModeBase();

public:
	UPROPERTY(Transient)
	//TSubclassOf<class ALMPawnBase> LMPawnBaseClass; 
	TSubclassOf<class ALMPawnPlayer> LMPawnPlayerClass;

	UPROPERTY(Transient)
	class APlayerStart* PlayerStart1P; 

	UPROPERTY(Transient)
	class APlayerStart* PlayerStart2P;

	UPROPERTY(Transient)
	TSubclassOf<class ATileGenerator> TileGeneratorClass; 

	TSubclassOf<class AGameManager> GameManagerClass;	

	TSubclassOf<class ASkillLightningAttack> LightningAtkClass;

	TSubclassOf<class ASkillStunAttack> StunAttackClass; 
	TSubclassOf<class ALMPlayerController> PlayerControllerClass;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score", meta = (AllowPrivateAccess = "true"))
	FGameSatus StatusP1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score", meta = (AllowPrivateAccess = "true"))
	FGameSatus StatusP2;
	
private:
	
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
	void OnPlayerDead(int32 TargetIdx, int32 KillerIndex = -1);

	void SpawnPlayer(int32 TargetIdx);

	UFUNCTION(BlueprintCallable)
	void DoLightningAttack(int32 OwnerIndex);

	UFUNCTION(BlueprintCallable)
	void DoStunAttack(int32 OwnerIndex);

	FORCEINLINE
		ALMPawnPlayer* GetPlayerByIndex(int32 TargetIndex) { ALMPawnPlayer* ReturnValue;  TargetIndex == 0 ? ReturnValue = PawnPlayer1 : ReturnValue = PawnPlayer2; return ReturnValue; }
// 0514 한규 추가
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Item")
	void Event_OnPlayerDied(int inputIndex);
	virtual void Event_OnPlayerDied_Implementation(int inputIndex);


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Item")
	void AdditionalEvent_OnGameEnded();
	virtual void AdditionalEvent_OnGameEnded_Implementation();

	UFUNCTION(BlueprintCallable)
	void ShowOrCreateMainHUD(bool bShow);

public :


	// 0512 한규 추가
  // BeginPlay 등에서 CreateWidget 으로 만든 인스턴스를 그대로 돌려주는 함수
	UFUNCTION(BlueprintPure, Category = "UI")
	UMainHUDWidget* GetMainHUD() const { return MainHUD; }

	// CDO가 완전히 만들어진 뒤(=블루프린트 기본값 로딩 완료 후)에 호출됩니다.
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

	void UpdatePlayerStatus();

protected:
	// BeginPlay() 에서 실제로 생성한 MainHUD
	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	UMainHUDWidget* MainHUD;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UMainHUDWidget> MainHUDWidgetClass;

	protected :
};

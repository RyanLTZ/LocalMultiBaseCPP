// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pawn/LMPawnBase.h"
#include "InputActionValue.h"
#include "../Map/SpawItemBase.h"
#include "LMPawnPlayer.generated.h"

/**
 * 
 */
UCLASS()
class LOCALMULTIBASECPP_API ALMPawnPlayer : public ALMPawnBase
{
	GENERATED_BODY()
public:
	ALMPawnPlayer();
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	int32 PlayerIndex = -1; 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* IMC_LMPlayerInput; 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* IA_LMMove1P; 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* IA_FIRE1P;

public:
	UFUNCTION(BlueprintCallable, Category = "Player")
	FORCEINLINE int32 GetPlayerIndex() const { return PlayerIndex; }

	UFUNCTION(BlueprintCallable, Category = "Player")
	FORCEINLINE void SetPlayerIndex(int32 NewIndex) { PlayerIndex = NewIndex; }

	UPROPERTY(EditAnywhere, Category = "Components")
	class USkeletalMeshComponent* SKComponent;

	UPROPERTY(EditAnywhere, Category = "Components")
	class UArrowComponent* FirePosition2;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProjectileObject> ProjectileClass; //Inherited class can be assigned


protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void BindInputActions(class UEnhancedInputComponent* EnhacedInputComponent);
	void OnInputMove(const FInputActionValue& Value);
	void HandlePlayerSpecificPossession();	

public:
	UFUNCTION(BlueprintCallable)
	void SetDamage(int32 Damage);

	UFUNCTION(BlueprintCallable)
	void Fire();
		

	void OnChargedEnerge();

	UFUNCTION(BlueprintCallable)
	void DoDie();

	UFUNCTION(BlueprintCallable)
	void DoLightningAttack();

	UFUNCTION(BlueprintCallable)
	void DoStunAttack();

	UFUNCTION(BlueprintCallable)
	void DoTileTake();

	void OnItemAquired(class ASpawItemBase* TargetItem);			
	void UpdateInventory();
	void MoveInputFromOutside(const FInputActionValue& Value);
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE
	void SetHitPlayer(int32 AttackPlayerIndex) { AttackPlayer = AttackPlayerIndex; }
	
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Normal Attack")
	float RechargeTime = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Normal Attack")
	int32 MaxChargedEnergy = 3; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Normal Attack")
	int32 ConsumeEnergyForAttack = 1; 		
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Normal Attack")
	int32 CurrentEnergy = MaxChargedEnergy;

	int32 AttackPlayer = -1;

public :

// 0520 한규 추가
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Hp")
	void AfterEvent_OnFire();
	virtual void AfterEvent_OnFire_Implementation();

// 0514 한규 추가
public :
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Hp")
	void Refresh_HpState();
	virtual void Refresh_HpState_Implementation();



// 0512 한규 추가
private :
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory",
		meta = (AllowPrivateAccess = "true"))
	TArray<ELMItemType> myItemInventory;
public:
	// Get Item
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Item")
	void PlayGetItemSound();
	virtual void PlayGetItemSound_Implementation();



	// Get Item
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Item")
	void GetItem(ELMItemType inputItemAttribute);
	virtual void GetItem_Implementation(ELMItemType inputItemAttribute);

	// Sub Function

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Item")
	void GetItem_LightningAttack();
	virtual void GetItem_LightningAttack_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Item")
	void GetItem_Fireball();
	virtual void GetItem_Fireball_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Item")
	void GetItem_ObstacleDestroyer();
	virtual void GetItem_ObstacleDestroyer_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Item")
	void GetItem_TileTaker();
	virtual void GetItem_TileTaker_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Item")
	void GetItem_BuffItem();
	virtual void GetItem_BuffItem_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Item")
	void GetItem_DebuffItem();
	virtual void GetItem_DebuffItem_Implementation();

	// UI

	/** Blueprint에서도 오버라이드 가능한 RefreshUI */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "UI")
	void RefreshUI();
	virtual void RefreshUI_Implementation();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pawn/LMPawnBase.h"
#include "InputActionValue.h"
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

	UFUNCTION(BlueprintCallable)
	void DoDie();
	
};

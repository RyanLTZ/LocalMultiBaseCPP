// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "LMPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class LOCALMULTIBASECPP_API ALMPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ALMPlayerController();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Player")
	class ALMPawnPlayer* Player2P;

	UPROPERTY(VisibleAnywhere, Category = "Input")
	class UInputAction* IA_LMMove2P;

public:
	UFUNCTION(BlueprintCallable, Category = "Player")
	void SetPlayer2P(class ALMPawnPlayer* const NewPlayer2P );

protected:
	virtual void SetupInputComponent() override;	

private:
	void OnInputMove2P(const FInputActionValue& Value);
};

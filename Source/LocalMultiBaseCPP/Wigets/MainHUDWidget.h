// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class LOCALMULTIBASECPP_API UMainHUDWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Player1Score;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Player2Score;	

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* RemainTime; 


public:
	void UpdateSocre(int32 PlayerIndex, int32 Score);
	void UpdateTimer(float Time);
	
};

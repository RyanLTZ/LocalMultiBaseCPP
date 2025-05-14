// Fill out your copyright notice in the Description page of Project Settings.


#include "Wigets/MainHUDWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"


void UMainHUDWidget::UpdateSocre(int32 PlayerIndex, int32 Score)
{
	if (PlayerIndex == 0)
	{
		Player1Score->SetText(FText::AsNumber(Score));
	}
	else if (PlayerIndex == 1)
	{
		Player2Score->SetText(FText::AsNumber(Score));
	}
}

void UMainHUDWidget::UpdateTimer(float Time)
{	
	FString FormattedString = FString::Printf(TEXT("%.1f"), Time);
	FText DisplayText = FText::FromString(FormattedString);
	if(nullptr != RemainTime)
	RemainTime->SetText(DisplayText);
}

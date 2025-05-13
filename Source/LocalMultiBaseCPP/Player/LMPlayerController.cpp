// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/LMPlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Pawn/LMPawnPlayer.h"
#include "ProjectileObject.h"
#include "Kismet/GameplayStatics.h"

ALMPlayerController::ALMPlayerController()
{	
	static ConstructorHelpers::FObjectFinder<UInputAction> IAObj(TEXT("'/Game/Inputs/IA_LMMove2P.IA_LMMove2P'"));
	
	if (IAObj.Succeeded() )
	{
		IA_LMMove2P = IAObj.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> IAFireObj(TEXT("'/Game/Inputs/IA_Fire2P.IA_Fire2P'"));
	if (IAFireObj.Succeeded())
	{
		IA_LMFire2P = IAFireObj.Object;
	}
}

void ALMPlayerController::SetPlayer2P(ALMPawnPlayer* const NewPlayer2P)
{
	ensure(NewPlayer2P);
	Player2P = NewPlayer2P;	
}

void ALMPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (EnhancedInputComponent && IA_LMMove2P)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetupInputComponent from Player2 COntroller"));
		EnhancedInputComponent->BindAction(IA_LMMove2P, ETriggerEvent::Triggered, this, &ALMPlayerController::OnInputMove2P);				

		if (IA_LMFire2P)
		{
			EnhancedInputComponent->BindAction(IA_LMFire2P, ETriggerEvent::Started, this, &ALMPlayerController::OnInputFire2P);
		}
	}	
}

void ALMPlayerController::OnInputMove2P(const FInputActionValue& Value)
{
	if (Player2P)
	{		
		FVector2D MoveVector = Value.Get<FVector2D>();
		//UE_LOG(LogTemp, Warning, TEXT("Player2 Input From Controller : %f, %f"), MoveVector.X, MoveVector.Y);
		FVector ForwardDirection = Player2P->GetActorForwardVector() * MoveVector.Y;
		FVector RightDirection = Player2P->GetActorRightVector() * MoveVector.X;
		FVector MoveDirection = ForwardDirection + RightDirection;

		if (MoveVector.X == 1)
		{
			Player2P->MeshComponent->SetWorldRotation(FRotator(0, 0, 0));
		}
		else if (MoveVector.X == -1)
		{
			Player2P->MeshComponent->SetWorldRotation(FRotator(0, 180, 0));
		}
		else if (MoveVector.Y == 1)
		{
			Player2P->MeshComponent->SetWorldRotation(FRotator(0, 270, 0));
		}
		else if (MoveVector.Y == -1)
		{
			Player2P->MeshComponent->SetWorldRotation(FRotator(0, 90, 0));
		}

		if (MoveDirection.SizeSquared() > 0.f)
		{
			Player2P->AddMovementInput(MoveDirection.GetSafeNormal());
		}
	}
}

void ALMPlayerController::OnInputFire2P()
{
	UE_LOG(LogTemp, Warning, TEXT("Player2P Fire"));
	Player2P->Fire();
	//UGameplayStatics::PlaySound2D(GetWorld(), FireSound);
}

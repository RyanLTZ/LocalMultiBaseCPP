// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawn/LMPawnPlayer.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "Player/LMPlayerController.h"
#include "Components/BoxComponent.h"

ALMPawnPlayer::ALMPawnPlayer()
{
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMCObj(TEXT("'/Game/Inputs/IMC_LMPlayerInput.IMC_LMPlayerInput'"));
	
	if (IMCObj.Succeeded())
	{
		IMC_LMPlayerInput = IMCObj.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> Input1PObj(TEXT("'/Game/Inputs/IA_LMMove1P.IA_LMMove1P'"));
	if (Input1PObj.Succeeded())
	{
		IA_LMMove1P = Input1PObj.Object;
	}	

	BoxComponent->SetGenerateOverlapEvents(true);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxComponent->SetCollisionObjectType(ECC_GameTraceChannel1);
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Block);
	BoxComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Block);
	//BoxComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
}

void ALMPawnPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	APlayerController* PlayerController = Cast<APlayerController>(NewController);
	if (PlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem< UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (SubSystem && IMC_LMPlayerInput)
		{
			SubSystem->AddMappingContext(IMC_LMPlayerInput, 0);
			UE_LOG(LogTemp, Warning, TEXT("Added Input MappingContext Index %d"), GetPlayerIndex());
		}
	}
}

void ALMPawnPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);	

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		BindInputActions(EnhancedInputComponent);		
	}

	HandlePlayerSpecificPossession();
}

void ALMPawnPlayer::BindInputActions(UEnhancedInputComponent* EnhacedInputComponent)
{
	if (PlayerIndex == 0 && IA_LMMove1P)
	{
		EnhacedInputComponent->BindAction(IA_LMMove1P, ETriggerEvent::Triggered, this, &ALMPawnPlayer::OnInputMove);
	}	
}

void ALMPawnPlayer::OnInputMove(const FInputActionValue& Value)
{
	FVector2D MoveVector = Value.Get<FVector2D>();
	//UE_LOG(LogTemp, Warning, TEXT("Player2 Input From Controller : %f, %f"), MoveVector.X, MoveVector.Y);
	FVector ForwardDirection = GetActorForwardVector() * MoveVector.Y;
	FVector RightDirection = GetActorRightVector() * MoveVector.X;
	FVector MoveDirection = ForwardDirection + RightDirection;

	if (MoveDirection.SizeSquared() > 0.f)
	{
		AddMovementInput(MoveDirection.GetSafeNormal());
	}
}

void ALMPawnPlayer::HandlePlayerSpecificPossession()
{
	if (PlayerIndex == 1)
	{		
		ALMPlayerController* LMPlayerController = Cast<ALMPlayerController>(GetWorld()->GetFirstLocalPlayerFromController()->GetPlayerController(GetWorld()));
		
		if (LMPlayerController)
		{
			LMPlayerController->SetPlayer2P(this);
		}
			
	}
}

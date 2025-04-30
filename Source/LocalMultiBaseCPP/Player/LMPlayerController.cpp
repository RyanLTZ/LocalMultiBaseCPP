// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/LMPlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Pawn/LMPawnPlayer.h"

ALMPlayerController::ALMPlayerController()
{	
	static ConstructorHelpers::FObjectFinder<UInputAction> IAObj(TEXT("'/Game/Inputs/IA_LMMove2P.IA_LMMove2P'"));
	
	if (IAObj.Succeeded() )
	{
		IA_LMMove2P = IAObj.Object;
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

		if (MoveDirection.SizeSquared() > 0.f)
		{
			Player2P->AddMovementInput(MoveDirection.GetSafeNormal());
		}
	}
}


// 0429 + Network

void ALMPlayerController::BP_JoinWithSearchResult(const FBlueprintSessionResult& SearchResult)
{
	// 온라인 서브시스템 인터페이스 가져오기
	IOnlineSubsystem* OSS = IOnlineSubsystem::Get();
	IOnlineSessionPtr SessInt = OSS ? OSS->GetSessionInterface() : nullptr;
	if (!SessInt.IsValid())
	{
		//UE_LOG(LogTemp, Warning, TEXT("세션 인터페이스를 얻지 못했습니다"));
		return;
	}

	// SearchResult 에서 실제 세션 이름을 꺼냅니다
	FName RealSessionName = TEXT("MyLANSession");

	// Resolve
	FString ConnectURL;
	SessInt->GetResolvedConnectString(RealSessionName, ConnectURL);

	// 로그 찍어보기
	UE_LOG(LogTemp, Warning, TEXT("Resolved Connect URL: '%s'"), *ConnectURL);

	// URL 이 비어있지 않을 때만 Travel 호출
	if (!ConnectURL.IsEmpty())
	{
		ClientTravel(*ConnectURL, TRAVEL_Absolute);
	}

}
// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawn/LMPawnPlayer.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "Player/LMPlayerController.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/ProjectileObject.h"
#include "EngineUtils.h"
#include "Game/LMGameModeBase.h"

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

	static ConstructorHelpers::FClassFinder<AProjectileObject> BP_Bullet(TEXT("'/Game/Blueprints/BP_Bullet.BP_Bullet_C'"));
	if (BP_Bullet.Succeeded())
	{
		ProjectileClass = BP_Bullet.Class;
	}

	
	BoxComponent->SetGenerateOverlapEvents(true);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxComponent->SetCollisionObjectType(ECC_GameTraceChannel1);
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Block);
	BoxComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Block);
	//BoxComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
		
	//UArrowComponent* ArrowForFirePos = Cast<UArrowComponent>(UGameplayStatics::GetActorOfClass(GetWorld(), UArrowComponent::StaticClass()));
	//if (ArrowForFirePos)
	//{
	//	FirePosition2 = ArrowForFirePos;
	//}		
	FirePosition2 = CreateDefaultSubobject<UArrowComponent>(TEXT("Fire Position2"));
	FirePosition2->SetupAttachment(MeshComponent);
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
	if (MoveVector.X == 1)
	{		
		MeshComponent->SetWorldRotation(FRotator(0, 0, 0));
	}
	else if (MoveVector.X == -1)
	{		
		MeshComponent->SetWorldRotation(FRotator(0, 180, 0));
	}
	else if (MoveVector.Y == 1)
	{	
		MeshComponent->SetWorldRotation(FRotator(0, 270, 0));
	}
	else if (MoveVector.Y == -1)
	{	
		MeshComponent->SetWorldRotation(FRotator(0, 90, 0));
	}	

	

	FVector ForwardDirection = GetActorForwardVector() * MoveVector.Y;
	FVector RightDirection = GetActorRightVector() * MoveVector.X;
	FVector MoveDirection = ForwardDirection + RightDirection;
		
	if (MoveDirection.SizeSquared() > 0.f)
	{
		AddMovementInput(MoveDirection.GetSafeNormal());					
	}	 
		

	//FQuat CurrentQuat = FQuat(GetActorRotation());

	//FQuat DeltaRotation = FQuat::MakeFromEuler(MoveDirection);
	//FQuat NewRotation = DeltaRotation * CurrentQuat;

	//SetActorRotation(NewRotation.Rotator(), ETeleportType::None);


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

void ALMPawnPlayer::Fire()
{
	AProjectileObject* Bullet = GetWorld()->SpawnActor<AProjectileObject>(ProjectileClass, FirePosition2->GetComponentLocation(), FirePosition2->GetComponentRotation());
}

void ALMPawnPlayer::DoDie()
{
	AGameModeBase* CurrentMode = GetWorld()->GetAuthGameMode();
	ALMGameModeBase* GameMode = Cast<ALMGameModeBase>(CurrentMode);
	if (GameMode)
	{
		GameMode->OnPlayerDead(PlayerIndex);
	}
}

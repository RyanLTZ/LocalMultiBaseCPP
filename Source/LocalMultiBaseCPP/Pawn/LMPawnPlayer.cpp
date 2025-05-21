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

	static ConstructorHelpers::FObjectFinder<UInputAction> Input1PFireObj(TEXT("'/Game/Inputs/IA_Fire.IA_Fire'"));
	if (Input1PObj.Succeeded())
	{
		IA_FIRE1P = Input1PFireObj.Object;
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

	if (PlayerIndex == 0 && IA_FIRE1P)
	{
		EnhacedInputComponent->BindAction(IA_FIRE1P, ETriggerEvent::Started, this, &ALMPawnPlayer::Fire);
	}	
}


void ALMPawnPlayer::OnInputMove(const FInputActionValue& Value)
{
	if (CurrentDebuffStatus == ELMDebuffType::Stun)
		return;

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

void ALMPawnPlayer::SetDamage(int32 Damage)
{
	if (bIsVulnaerable)
		return;

	UE_LOG(LogTemp, Warning, TEXT("Taking Damage %d"), Damage);

	Hp -= Damage;

	if (Hp < 0)
	{
		Hp = 0;
		this->DoDie();
	}	

	Refresh_HpState();
}

void ALMPawnPlayer::Fire()
{
	
	if (CurrentEnergy == 0)
		return;

	FActorSpawnParameters SpawnParams;	

	AProjectileObject* Bullet = Cast<AProjectileObject>(UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), ProjectileClass, FirePosition2->GetComponentTransform()));
	if (Bullet)
	{
		Bullet->SetPlayerIndex(PlayerIndex);
		Bullet->SetActorRotation(FirePosition2->GetComponentRotation());

		UGameplayStatics::FinishSpawningActor(Bullet, FirePosition2->GetComponentTransform());

		CurrentEnergy--;
		if (CurrentEnergy < MaxChargedEnergy)
		{
			//FTimerHandle Handle = GetWorldTimerManager().GenerateHandle(0);
			GetWorldTimerManager().SetTimer(HandleForEnergeyCharge, this, &ALMPawnPlayer::OnChargedEnerge, RechargeTime, false, -1);
		}

		RefreshUI();

		AfterEvent_OnFire();
	}	
}

void ALMPawnPlayer::OnChargedEnerge()
{	
	//FTimerHandle Handle = GetWorldTimerManager().GenerateHandle(0);
	GetWorldTimerManager().ClearTimer(HandleForEnergeyCharge);
	CurrentEnergy = FMath::Min(++CurrentEnergy, MaxChargedEnergy);
	RefreshUI();

	if (CurrentEnergy < MaxChargedEnergy)
	{		
		GetWorldTimerManager().SetTimer(HandleForEnergeyCharge, this, &ALMPawnPlayer::OnChargedEnerge, RechargeTime, false, -1);
	}
}

void ALMPawnPlayer::DoDie()
{
	if (bIsVulnaerable)
		return;

	AGameModeBase* CurrentMode = GetWorld()->GetAuthGameMode();
	ALMGameModeBase* GameMode = Cast<ALMGameModeBase>(CurrentMode);
	if (GameMode)
	{
		GameMode->OnPlayerDead(PlayerIndex, AttackPlayer);
		AttackPlayer = -1;
	}

	RefreshUI();
}

void ALMPawnPlayer::DoLightningAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("%d"), PlayerIndex);
	UpdateInventory();
	//if (myItemInventory.Num() > 0 && myItemInventory.Contains(ELMItemType::LightningAttack))
	{
		AGameModeBase* CurrentMode = GetWorld()->GetAuthGameMode();
		ALMGameModeBase* GameMode = Cast<ALMGameModeBase>(CurrentMode);
		if (GameMode)
		{
			GameMode->DoLightningAttack(PlayerIndex);
		}
	}
	
}

void ALMPawnPlayer::DoStunAttack()
{
	//if (myItemInventory.Num() > 0 && myItemInventory.Contains(ELMItemType::LightningAttack))
	{
		AGameModeBase* CurrentMode = GetWorld()->GetAuthGameMode();
		ALMGameModeBase* GameMode = Cast<ALMGameModeBase>(CurrentMode);
		if (GameMode)
		{
			GameMode->DoStunAttack(PlayerIndex);
		}
	}

	RefreshUI();
}

void ALMPawnPlayer::DoTileTake()
{
	
}

void ALMPawnPlayer::OnItemAquired(ASpawItemBase* TargetItem)
{
	
}

void ALMPawnPlayer::UpdateInventory()
{
	
}

void ALMPawnPlayer::MoveInputFromOutside(const FInputActionValue& Value)
{
	OnInputMove(Value);
}

void ALMPawnPlayer::AfterEvent_OnFire_Implementation()
{
}

void ALMPawnPlayer::Refresh_HpState_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Hp : %d"), Hp);
	if (Hp <= 0)
	{
		this->DoDie();
	}
}

// 0512 한규 추가
void ALMPawnPlayer::RefreshUI_Implementation()
{
	// C++ 기본 동작 (예: 바인딩된 데이터로 텍스트 갱신)
	// ...

	// (블루프린트 오버라이드가 없으면 이 함수만 실행됩니다)
}


void ALMPawnPlayer::PlayGetItemSound_Implementation()
{
}

void ALMPawnPlayer::GetItem_Implementation(ELMItemType inputItemAttribute)
{
	// 기본값 할당
	if (0 == myItemInventory.Num())
	{
		myItemInventory.Add(ELMItemType::None);
		myItemInventory.Add(ELMItemType::None);
	}

	if (myItemInventory[0] == ELMItemType::None)
	{
		myItemInventory[0] = inputItemAttribute;
	}
	else if (myItemInventory[1] == ELMItemType::None)
	{
		myItemInventory[1] = inputItemAttribute;
	}
	else
		return;

	switch (inputItemAttribute)
	{
	case ELMItemType::LightningAttack :
		GetItem_LightningAttack();
		break;
	case ELMItemType::Fireball :
		GetItem_Fireball();
		break;
	case ELMItemType::ObstacleDestroyer :
		GetItem_ObstacleDestroyer();
		break;
	case ELMItemType::TileTaker :
		GetItem_TileTaker();
		break;
	case ELMItemType::BuffItem :
		GetItem_BuffItem();
		break;
	case ELMItemType::DebuffItem :
		GetItem_DebuffItem();
		break;
	}

	RefreshUI();
}

void ALMPawnPlayer::GetItem_LightningAttack_Implementation()
{
	//myItemInventory.Add(ELMItemType::LightningAttack);
}

void ALMPawnPlayer::GetItem_Fireball_Implementation()
{
	//myItemInventory.Add(ELMItemType::Fireball);
}

void ALMPawnPlayer::GetItem_ObstacleDestroyer_Implementation()
{
	//myItemInventory.Add(ELMItemType::ObstacleDestroyer);
}

void ALMPawnPlayer::GetItem_TileTaker_Implementation()
{
	//myItemInventory.Add(ELMItemType::TileTaker);
}

void ALMPawnPlayer::GetItem_BuffItem_Implementation()
{
	
}

void ALMPawnPlayer::GetItem_DebuffItem_Implementation()
{
	
}

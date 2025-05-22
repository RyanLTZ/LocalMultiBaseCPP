// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawn/LMPawnBase.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/SkeletalMeshComponent.h"
#include "EngineUtils.h"
#include "Game/LMGameModeBase.h"

// Sets default values
ALMPawnBase::ALMPawnBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

    BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
    SetRootComponent(BoxComponent);
    BoxComponent->SetBoxExtent(FVector(50.f, 50.f, 50.f));

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(BoxComponent);

    //Input
    PawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("PawnMovement"));
    PawnMovement->MaxSpeed = MaxSpeed;
    PawnMovement->Acceleration = Acceleration;
    PawnMovement->Deceleration = Deceleration;
    PawnMovement->TurningBoost = TurningBoost;            
}

//void ALMPawnBase::BeginPlay()
//{
//    CurrentHandle = GetWorldTimerManager().GenerateHandle(0);
//}

void ALMPawnBase::SetDamage(int32 Damage)
{ 
}

void ALMPawnBase::DoDie()
{    
}

void ALMPawnBase::ApplyBuffDebuff()
{
    InitLMPawnStatus();
    if (FMath::RandRange(0, 1) > 0)
    {
        PawnMovement->MaxSpeed *= 2;
        PawnMovement->Acceleration *= 1.5f;
    }
    else
    {
        PawnMovement->MaxSpeed *= 0.3f;
        PawnMovement->Acceleration *= 0.5f;

    }
   
    float EffectDuration = 5;
    GetWorldTimerManager().ClearTimer(CurrentHandleForBuffDebuff);
    GetWorldTimerManager().SetTimer(CurrentHandleForBuffDebuff, this, &ALMPawnBase::OnFinishBuffDebuffEffect, EffectDuration, false, -1);
}

void ALMPawnBase::OnFinishBuffDebuffEffect()
{
    UE_LOG(LogTemp, Warning, TEXT("Item Effect End"));
    InitLMPawnStatus();
}

void ALMPawnBase::InitLMPawnStatus()
{
    PawnMovement->MaxSpeed = MaxSpeed;
    PawnMovement->Acceleration = Acceleration;
    PawnMovement->Deceleration = Deceleration;
    PawnMovement->TurningBoost = TurningBoost;
    bIsVulnaerable = false; 
    CurrentBuffStatus = ELMBuffType::None;
    CurrentDebuffStatus = ELMDebuffType::None;    
    GetWorldTimerManager().ClearTimer(CurrentHandleForBuffDebuff);

    Refresh_BuffState(false);        
}

void ALMPawnBase::SetBuff(ELMBuffType Buff, UBuffDebuff* BuffDebuffData)
{    
    UE_LOG(LogTemp, Warning, TEXT("Buff Type : %d, BuffEffect : %f"), Buff, BuffDebuffData->GetBuffDebuffData().MoveSpeedBuffFacor);
    UE_LOG(LogTemp, Warning, TEXT("Buff Type : %d, BuffEffect : %d"), Buff, BuffDebuffData->GetBuffDebuffData().bIsVulnaerable);
    UE_LOG(LogTemp, Warning, TEXT("Buff Type : %d, BuffEffect : %d"), Buff, BuffDebuffData->GetBuffDebuffData().HealHPValue);
    InitLMPawnStatus();
    bIsVulnaerable = BuffDebuffData->GetBuffDebuffData().bIsVulnaerable;
    PawnMovement->MaxSpeed *= BuffDebuffData->GetBuffDebuffData().MoveSpeedBuffFacor;
    if (Hp < MaxHp)
    {
        Hp += BuffDebuffData->GetBuffDebuffData().HealHPValue;
    }
     
    CurrentBuffStatus = Buff;

    
    if (BuffDebuffData->GetBuffDebuffData().Duration > 0)
    {   
        GetWorldTimerManager().SetTimer(CurrentHandleForBuffDebuff, this, &ALMPawnBase::OnFinishBuffDebuffEffect, BuffDebuffData->GetBuffDebuffData().Duration, false, -1);
    } 

    BuffDebuffData->MarkAsGarbage();

    Refresh_BuffState(true);

    AGameModeBase* CurrentMode = GetWorld()->GetAuthGameMode();
    ALMGameModeBase* GameMode = Cast<ALMGameModeBase>(CurrentMode);
    if (CurrentMode && GameMode)
    {
        GameMode->UpdatePlayerStatus();
    }
    
}

void ALMPawnBase::SetDebuff(ELMDebuffType Buff, UBuffDebuff* BuffDebuffData)
{
    InitLMPawnStatus();    
    Hp -= BuffDebuffData->GetBuffDebuffData().DecreaseHPValue;    
    PawnMovement->MaxSpeed *= BuffDebuffData->GetBuffDebuffData().MoveSpeedDebuffFactor;
    CurrentDebuffStatus = Buff;     
        
    if (BuffDebuffData->GetBuffDebuffData().Duration > 0)
    {   
        GetWorldTimerManager().SetTimer(CurrentHandleForBuffDebuff, this, &ALMPawnBase::OnFinishBuffDebuffEffect, BuffDebuffData->GetBuffDebuffData().Duration, false, -1);
    }          
    BuffDebuffData->MarkAsGarbage();
    Refresh_BuffState(true);        

    AGameModeBase* CurrentMode = GetWorld()->GetAuthGameMode();
    ALMGameModeBase* GameMode = Cast<ALMGameModeBase>(CurrentMode);
    if (CurrentMode && GameMode)
    {
        GameMode->UpdatePlayerStatus();
    }
}

void ALMPawnBase::Refresh_BuffState_Implementation(bool isShow)
{
}


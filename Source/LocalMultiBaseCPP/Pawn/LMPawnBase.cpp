// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawn/LMPawnBase.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/SkeletalMeshComponent.h"
#include "EngineUtils.h"

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
    FTimerHandle CurrentHandle = GetWorldTimerManager().GenerateHandle(0);    
    GetWorldTimerManager().SetTimer(CurrentHandle, this, &ALMPawnBase::OnFinishBuffDebuffEffect, EffectDuration, false, -1);
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
}

void ALMPawnBase::SetBuff(ELMBuffType Buff, FBuffDebuffData& BuffDebuffData)
{
    InitLMPawnStatus();
    bIsVulnaerable = BuffDebuffData.bIsVulnaerable;
    PawnMovement->MaxSpeed *= BuffDebuffData.MoveSpeedBuffFacor;
    Hp += BuffDebuffData.HealHPValue >= MaxHp ? MaxHp : Hp + BuffDebuffData.HealHPValue;
}

void ALMPawnBase::SetDebuff(ELMDebuffType Buff, FBuffDebuffData& BuffDebuffData)
{
    InitLMPawnStatus();    
    Hp -= BuffDebuffData.DecreaseHPValue;
    PawnMovement->MaxSpeed *= BuffDebuffData.MoveSpeedDebuffFactor;    
    CurrentDebuffStatus = Buff;
}


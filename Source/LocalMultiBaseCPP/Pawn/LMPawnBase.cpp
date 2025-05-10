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
    PawnMovement->MaxSpeed *= 0.1f;
    PawnMovement->Acceleration *= 0.2f;
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
}


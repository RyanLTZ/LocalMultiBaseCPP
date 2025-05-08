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


    //for (TActorIterator<USkeletalMeshComponent> It(GetWorld()); It; ++It)
    //{
    //    USkeletalMeshComponent* FoundStart = *It;
    //    if (FoundStart)
    //    {
    //        SKMComponent = FoundStart;
    //        break;
    //    }
    //}

    //if (SKMComponent)
    //{
    //    UE_LOG(LogTemp, Warning, TEXT("SKM created"));
    //}

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(BoxComponent);
    //
    //static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("'/Engine/BasicShapes/Cube.Cube'"));    
    //if (CubeMesh.Succeeded())
    //{
    //    MeshComponent->SetStaticMesh(CubeMesh.Object);
    //}

    //Input
    PawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("PawnMovement"));
    PawnMovement->MaxSpeed = 500.f; 
    PawnMovement->Acceleration = 2048.f;
    PawnMovement->Deceleration = 2000.f;
    PawnMovement->TurningBoost = 8.f;    
}

void ALMPawnBase::SetDamage(int32 Damage)
{
    Hp -= Damage;

    if (Hp < 0)
    {
        Hp = 0;
        DoDie();
    }                
}

void ALMPawnBase::DoDie()
{
}


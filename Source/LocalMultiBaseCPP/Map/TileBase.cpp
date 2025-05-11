// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/TileBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Game/LMGameModeBase.h"

// Sets default values
ATileBase::ATileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;
    BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
    SetRootComponent(BoxComponent);   

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(BoxComponent);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("'/Engine/BasicShapes/Cube.Cube'"));    
    if (CubeMesh.Succeeded())
    {
        MeshComponent->SetStaticMesh(CubeMesh.Object);
    }

    BoxExtentSize.Set(BoxComponent->GetUnscaledBoxExtent().X, BoxComponent->GetUnscaledBoxExtent().Y, BoxComponent->GetUnscaledBoxExtent().Z);
}

// Called when the game starts or when spawned
void ATileBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATileBase::UpatePlayerScore(int32 TargetPlayerIndex)
{
    if (TargetPlayerIndex != OccupiedPlayerIndex)
    {
        AGameModeBase* CurrentMode = GetWorld()->GetAuthGameMode();
        ALMGameModeBase* GameMode = Cast<ALMGameModeBase>(CurrentMode);
        if (CurrentMode && GameMode)
        {            
            if (OccupiedPlayerIndex != -1)
            { 
                GameMode->OnSubScore(OccupiedPlayerIndex);
            }            
            GameMode->OnAddScore(TargetPlayerIndex);
            OccupiedPlayerIndex = TargetPlayerIndex;
        }

    }
}

void ATileBase::OccupyTile(int32 TargetPlayerIndex)
{
    MeshComponent->SetVectorParameterValueOnMaterials(TEXT("BaseColor"), FVector(0.119538, 0.665387, 0.095307));
    UpatePlayerScore(TargetPlayerIndex);
}


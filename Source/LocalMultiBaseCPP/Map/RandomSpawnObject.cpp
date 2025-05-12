// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/RandomSpawnObject.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ARandomSpawnObject::ARandomSpawnObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    //BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
    //SetRootComponent(BoxComponent);

    //MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    //MeshComponent->SetupAttachment(BoxComponent);

    //
    //static ConstructorHelpers::FObjectFinder<UStaticMesh> SMObject(TEXT("'/Game/Models/Object/Tomb/Tomb_1.Tomb_1'"));
    //if (SMObject.Succeeded())
    //{
    //    MeshComponent->SetStaticMesh(SMObject.Object);
    //}

    //BoxExtentSize.Set(BoxComponent->GetUnscaledBoxExtent().X, BoxComponent->GetUnscaledBoxExtent().Y, BoxComponent->GetUnscaledBoxExtent().Z);

}

// Called when the game starts or when spawned
void ARandomSpawnObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARandomSpawnObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


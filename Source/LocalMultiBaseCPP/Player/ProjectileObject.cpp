// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ProjectileObject.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Game/LMGameModeBase.h"
#include "Pawn/LMPawnPlayer.h"


// Sets default values
AProjectileObject::AProjectileObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	SetRootComponent(BoxComponent);
	BoxComponent->SetBoxExtent(FVector(50.f, 50.f, 50.f));
	BoxComponent->SetWorldScale3D(FVector(0.75f, 0.25f, 1.f));
	BoxComponent->SetCollisionProfileName(TEXT("Bullet"));

	SM_Component = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh_Bullet"));
	SM_Component->SetupAttachment(BoxComponent);

}

// Called when the game starts or when spawned
void AProjectileObject::BeginPlay()
{
	Super::BeginPlay();
	
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AProjectileObject::OnObjectOverlap);
}

// Called every frame
void AProjectileObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NewLocation = GetActorLocation() + GetActorForwardVector() * MoveSpeed * DeltaTime;

	SetActorLocation(NewLocation);

}

void AProjectileObject::OnObjectOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Bullet From Player %d"), OwnerPlayerIndex);
	ALMPawnPlayer* Player = Cast<ALMPawnPlayer>(OtherActor);
	if (Player)
	{
		if (Player->GetPlayerIndex() != this->OwnerPlayerIndex)
		{
			Player->SetHitPlayer(OwnerPlayerIndex);
			Player->SetDamage(DamageOnPlayer);
			this->Destroy();
		}
	}
	//if (Enemy != nullptr)
	//{
	//	Enemy->Destroy();

	//	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionFX, GetActorLocation(), GetActorRotation());

	//	AGameModeBase* CurrentMode = GetWorld()->GetAuthGameMode();
	//	AShootingGameModeBase* GameMode = Cast<AShootingGameModeBase>(CurrentMode);
	//	if (GameMode)
	//	{
	//		GameMode->AddScore(1);
	//	}
	//}	
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/LMCharacterPlayer.h"

// Sets default values
ALMCharacterPlayer::ALMCharacterPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALMCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALMCharacterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ALMCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


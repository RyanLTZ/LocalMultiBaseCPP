// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SkillStunAttack.h"

ASkillStunAttack::ASkillStunAttack()
{
	
}

void ASkillStunAttack::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Stun, C++ BeginPlay"));
}

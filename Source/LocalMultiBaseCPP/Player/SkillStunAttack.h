// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/SkillBase.h"
#include "SkillStunAttack.generated.h"

/**
 * 
 */
UCLASS()
class LOCALMULTIBASECPP_API ASkillStunAttack : public ASkillBase
{
	GENERATED_BODY()

public:
	ASkillStunAttack();

protected:
	virtual void BeginPlay() override;

	
};

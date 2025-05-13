// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/SkillBase.h"
#include "SkillLightningAttack.generated.h"

/**
 * 
 */
UCLASS()
class LOCALMULTIBASECPP_API ASkillLightningAttack : public ASkillBase
{
	GENERATED_BODY()

public:
	ASkillLightningAttack();

protected:
	virtual void BeginPlay() override;

	
};

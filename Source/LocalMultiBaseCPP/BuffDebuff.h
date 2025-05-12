// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BuffDebuff.generated.h"

UENUM()
enum class ELMBuffType : uint8
{
	None UMETA(DisplayName = "None"),
	MoveSpeed UMETA(DisplayName = "MoveSpeed"),
	Invulnerable UMETA(DisplayName = "Invulnerable"),
	Heal UMETA(DisplayName = "Heal"),
};

UENUM()
enum class ELMDebuffType : uint8
{
	None UMETA(DisplayName = "None"),
	Slow UMETA(DisplayName = "Slow"),
};

UENUM()
enum class ELMItemType : uint8
{
	None UMETA(DisplayName = "None"),
	LightningAttack UMETA(DisplayName = "LightingAttack"),
	Fireball UMETA(DisplayName = "FireBall"),
	ObstacleDestroyer UMETA(DisplayName = "ObstacleDestroyer"),
	TileTaker UMETA(DisplayName = "TileTaker"),
	BuffItem UMETA(DisplayName = "BuffItem"),
	DebuffItem UMETA(DisplayName = "DebuffItem"),
};
/**
 * 
 */
UCLASS()
class LOCALMULTIBASECPP_API UBuffDebuff : public UObject
{
	GENERATED_BODY()

public:
	UBuffDebuff(); //constructor 

public:
	
	
};

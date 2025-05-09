// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameCommon.generated.h"


#define LOG_PRINT(fmt, ...) UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::Printf(fmt, ##__VA_ARGS__))
#define LOG_ERROR(fmt, ...) UE_LOG(LogTemp, Error, TEXT("%s"), *FString::Printf(fmt, ##__VA_ARGS__))

UENUM()
enum class EItemType : uint8
{
	Buff = 0 UMETA(DisplayName = "Buff"),
	Debuff = 1  UMETA(DisplayName = "Debuff"),
	Attack = 2 UMETA(DisplayName = "Attack"),
};

UCLASS()
class LOCALMULTIBASECPP_API AGameCommon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameCommon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

};

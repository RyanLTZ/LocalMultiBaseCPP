 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SkillBase.generated.h"

UENUM()
enum class SkillType : uint8
{
	None,
	LightningAttack UMETA(DisplayName = "LightingAttack"),
	Fireball UMETA(DisplayName = "FireBall"),
	ObstacleDestroyer UMETA(DisplayName = "ObstacleDestroyer"),
	TileTaker UMETA(DisplayName = "TileTaker"),
	StunAttack UMETA(DisplayName = "StunAttack")
};

UCLASS()
class LOCALMULTIBASECPP_API ASkillBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASkillBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;	

	UFUNCTION(BlueprintCallable)
	FORCEINLINE		
		void SetOnwerPlayer(int32 Index) { OwnerPlayerIndex = Index; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE
		int32 GetOwnerPlayer() { return OwnerPlayerIndex; }

private:
	SkillType CurrentSkillType = SkillType::None;

protected:
	int32 OwnerPlayerIndex = -1;

};

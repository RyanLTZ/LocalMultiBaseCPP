// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "LMPawnBase.generated.h"



UCLASS()
class LOCALMULTIBASECPP_API ALMPawnBase : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ALMPawnBase();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* MeshComponent;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UFloatingPawnMovement* PawnMovement; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move Property")
	float MaxSpeed = 2000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move Property")
	float Acceleration = 2000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move Property")
	float Deceleration = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move Property")
	float TurningBoost = 8.f;

protected:
	int32 Hp = 100;

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE
	void SetHP(int32 HP) { Hp = HP; }		

	void SetDamage(int32 Damage);
	void DoDie();


	UFUNCTION(BlueprintCallable)
	void ApplyBuffDebuff();
	void OnFinishBuffDebuffEffect();

	UFUNCTION(BlueprintCallable)
	void InitLMPawnStatus();
};

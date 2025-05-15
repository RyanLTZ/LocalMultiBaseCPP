// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BuffDebuff.h"
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Property")
	int32 Hp = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Property")
	int32 MaxHp = 2;	
		

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE
		void SetHP(int32 inputHp) { Hp = inputHp; }

	void SetDamage(int32 Damage);
	void DoDie();

	UFUNCTION(BlueprintCallable)
	void ApplyBuffDebuff();
	void OnFinishBuffDebuffEffect();

	UFUNCTION(BlueprintCallable)
	void InitLMPawnStatus();

	UFUNCTION(BlueprintCallable)
	void SetBuff(ELMBuffType Buff, FBuffDebuffData& BuffDebuffData);

	UFUNCTION(BlueprintCallable)
	void SetDebuff(ELMDebuffType Buff, FBuffDebuffData& BuffDebuffData);

protected:
	ELMDebuffType CurrentDebuffStatus;
	ELMBuffType CurrentBuffStatus;
	bool bIsVulnaerable = false; 
};
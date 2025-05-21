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

protected:
	//virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* MeshComponent;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UFloatingPawnMovement* PawnMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move Property")
	float MaxSpeed = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move Property")
	float Acceleration = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move Property")
	float Deceleration = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move Property")
	float TurningBoost = 8.f;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Property")
	int32 Hp = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Property")
	int32 MaxHp = 3;	
		

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
	void SetBuff(ELMBuffType Buff, UBuffDebuff* BuffDebuffData);

	UFUNCTION(BlueprintCallable)
	void SetDebuff(ELMDebuffType Buff, UBuffDebuff* BuffDebuffData);

	//void InitTimer();
//
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Hp")
	void Refresh_BuffState(bool isShow);
	virtual void Refresh_BuffState_Implementation(bool isShow);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BuffDebuff");
	ELMDebuffType CurrentDebuffStatus;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BuffDebuff");
	ELMBuffType CurrentBuffStatus;

	bool bIsVulnaerable = false; 

private:
	FTimerHandle CurrentHandleForBuffDebuff;
};
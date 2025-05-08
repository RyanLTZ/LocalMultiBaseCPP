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
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	//class USkeletalMeshComponent* SKMComponent;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UFloatingPawnMovement* PawnMovement; 

private:
	int32 Hp; 

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE
	void SetHP(int32 HP) { Hp = HP; }

	UFUNCTION(BlueprintCallable)
	void SetDamage(int32 Damage);

	UFUNCTION(BlueprintCallable)
	void DoDie();

};

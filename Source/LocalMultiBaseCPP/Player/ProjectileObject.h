// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileObject.generated.h"

UCLASS()
class LOCALMULTIBASECPP_API AProjectileObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* SM_Component;

	UPROPERTY(EditAnywhere)
	float MoveSpeed = 800.f;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* ExplosionFX;

	UFUNCTION()
	void OnObjectOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE
	void SetPlayerIndex(int32 OwerIndex) { OwnerPlayerIndex = OwerIndex; }

private:
	int32 OwnerPlayerIndex = -1; 

protected:
	UPROPERTY(EditAnywhere, Category = "Property")
	int32 DamageOnPlayer = 1;

};

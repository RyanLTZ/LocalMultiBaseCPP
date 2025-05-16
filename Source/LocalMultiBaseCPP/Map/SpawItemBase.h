// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawItemBase.generated.h"

UENUM(BlueprintType)
enum class ELMItemType : uint8
{
	None,
	LightningAttack UMETA(DisplayName = "LightingAttack"),
	Fireball UMETA(DisplayName = "FireBall"),
	StunAttack UMETA(DisplayName = "StunAttack"),
	ObstacleDestroyer UMETA(DisplayName = "ObstacleDestroyer"),
	TileTaker UMETA(DisplayName = "TileTaker"),
	BuffItem UMETA(DisplayName = "BuffItem"),
	DebuffItem UMETA(DisplayName = "DebuffItem"),
	MaxBoundary, 
};

UCLASS()
class LOCALMULTIBASECPP_API ASpawItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawItemBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:

	UPROPERTY(EditAnywhere, Category = "Item");
	class UBuffDebuff* BuffDebuffData;

public:	

	// Called every frame
	virtual void Tick(float DeltaTime) override;	

	UFUNCTION(BlueprintCallable)
	void OnItemCollision(class ALMPawnPlayer* AquiredPlayer );
	
	UFUNCTION(BlueprintCallable)
	void GenerateItemData();

	// 0516 한규 추가
	UFUNCTION()
	void OnSphereOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

protected :
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item");
	ELMItemType myItemType;
};
 
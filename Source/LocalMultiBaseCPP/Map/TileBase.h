// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TileBase.generated.h"

UCLASS()
class LOCALMULTIBASECPP_API ATileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATileBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;	

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* MeshComponent;

	UPROPERTY(BlueprintReadWrite, Category = "Components")
	int32 OccupiedPlayerIndex = -1;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* BoxComponent;	

private:
	FVector BoxExtentSize = FVector::ZeroVector;
	

public:
	FORCEINLINE
	FVector GetBoxExtentSize() { return BoxExtentSize;	}
	
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE
	void SetOccupiedPlayerIndex(int32 PlayerIndex) { OccupiedPlayerIndex = PlayerIndex; }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE
	int32 GetOccupiedPlayerIndex() { return OccupiedPlayerIndex;  }

	UFUNCTION(BlueprintCallable)
	void UpatePlayerScore(int32 TargetPlayerIndex);

};

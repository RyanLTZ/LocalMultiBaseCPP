// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BuffDebuff.generated.h"

UENUM(BlueprintType)
enum class ELMBuffType : uint8
{
	None UMETA(DisplayName = "None"),
	MoveSpeed UMETA(DisplayName = "MoveSpeed"),
	Invulnerable UMETA(DisplayName = "Invulnerable"),
	Heal UMETA(DisplayName = "Heal"),	
	MaxBoundary,
};


UENUM(BlueprintType)
enum class ELMDebuffType : uint8
{
	None UMETA(DisplayName = "None"),
	Slow UMETA(DisplayName = "Slow"),
	HPDecrease UMETA(DisplayName = "HP Decrease"),
	Stun UMETA(DisplayName = "Stun"),
	MaxBoundary,
};


USTRUCT(BlueprintType)
struct FBuffDebuffData 
{	
	GENERATED_BODY()

	float MoveSpeedBuffFacor = 1.f;
	float MoveSpeedDebuffFactor = 1.f;
	int32 HealHPValue = 0;
	int32 DecreaseHPValue = 0;
	bool bIsVulnaerable = false; 
	float Duration = 0.f;

	FORCEINLINE
	void Init()
	{
		MoveSpeedBuffFacor = 1.f;
		MoveSpeedDebuffFactor = 1.f;
		HealHPValue = 0;
		DecreaseHPValue = 0;
		bIsVulnaerable = false;
		Duration = 0.f;
	};
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
	
private:
	ELMDebuffType CurrentDebuff = ELMDebuffType::None;
	ELMBuffType CurrentBuff = ELMBuffType::None;
	FBuffDebuffData ReturnData;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")	
	float MoveSpeedBuffFacor = 1.2f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	float MoveSpeedBuff2Factor = 2.f; 

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	float MoveSpeedDebuffFactor = 0.8f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	float MoveSpeedDebuff2Factor = 0.4f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Status")
	int32 HealHPValue = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Status")
	int32 DecreaseHPValue = 1;

public:
	FBuffDebuffData& GetBuffDebuffData();
	void GenerateBuff();
	void GenerateDebuff();
	
	FORCEINLINE
	ELMDebuffType GetDebuffType() { return CurrentDebuff; }

	FORCEINLINE
		ELMBuffType GetBuffType() { return CurrentBuff; }

};

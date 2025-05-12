// Fill out your copyright notice in the Description page of Project Settings.


#include "BuffDebuff.h"

UBuffDebuff::UBuffDebuff()
{
}

FBuffDebuffData* UBuffDebuff::GetBuffDebuffData()
{	
	//ReturnData.Init();
	//switch (CurrentBuff)
	//{
	//case ELMBuffType::None:
	//	break;
	//case ELMBuffType::MoveSpeed:
	//	ReturnData.MoveSpeedBuffFacor = MoveSpeedBuffFacor;
	//	break;
	//case ELMBuffType::Invulnerable:
	//	ReturnData.bIsVulnaerable = true; 
	//	break;
	//case ELMBuffType::Heal:
	//	ReturnData.HealHPValue = HealHPValue;
	//	break;
	//default:
	//	break;
	//}

	//switch (CurrentDebuff)
	//{
	//case ELMDebuffType::None:
	//	break;
	//case ELMDebuffType::Slow:
	//	ReturnData.HealHPValue = HealHPValue;
	//	break;
	//case ELMDebuffType::HPDecrease:
	//	ReturnData.DecreaseHPValue = DecreaseHPValue;
	//default:
	//	break;
	//}

	return &ReturnData;	
}

void UBuffDebuff::GenerateBuff()
{
	int32 RandomResult = FMath::RandRange((int32)ELMBuffType::None + 1, (int32)ELMBuffType::MaxBoundary - 1);
	ReturnData.Init();

	switch ((ELMBuffType)RandomResult)
	{
	case ELMBuffType::None:
		break;
	case ELMBuffType::MoveSpeed:
		ReturnData.MoveSpeedBuffFacor = MoveSpeedBuffFacor;
		break;
	case ELMBuffType::Invulnerable:
		ReturnData.bIsVulnaerable = true;
		break;
	case ELMBuffType::Heal:
		ReturnData.HealHPValue = HealHPValue;
		break;
	default:
		break;
	}

}

void UBuffDebuff::GenerateDebuff()
{
	int32 RandomResult = FMath::RandRange((int32)ELMDebuffType::None + 1, (int32)ELMDebuffType::MaxBoundary - 1);
	ReturnData.Init();

	switch ((ELMDebuffType)RandomResult)
	{
	case ELMDebuffType::None:
		break;
	case ELMDebuffType::Slow:
		ReturnData.MoveSpeedDebuffFactor = MoveSpeedDebuffFactor;
		break;
	case ELMDebuffType::HPDecrease:
		ReturnData.DecreaseHPValue = DecreaseHPValue;
		break;
	default:
		break;
	}
}

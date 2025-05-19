// Fill out your copyright notice in the Description page of Project Settings.


#include "BuffDebuff.h"

UBuffDebuff::UBuffDebuff()
{
}

FBuffDebuffData& UBuffDebuff::GetBuffDebuffData()
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

	return ReturnData;	
}

void UBuffDebuff::GenerateBuff()
{
	// (1) ����� ���� Ÿ���� �迭�� ����
	static const ELMBuffType BuffOptions[] = {
		ELMBuffType::MoveSpeed,
		ELMBuffType::Invulnerable,
		ELMBuffType::Heal
	};

	// ���� ��ȯ ����ü �ʱ�ȭ
	ReturnData.Init();

	// (2) 0~2 ���� �ε����� �����ϰ� �̰� �ش� enum ����
	int32 ChoiceIndex = FMath::RandRange(0, UE_ARRAY_COUNT(BuffOptions) - 1);
	ELMBuffType ChosenBuff = BuffOptions[ChoiceIndex];
	CurrentBuff = ChosenBuff;

	// (3) ���õ� ������ ���� ReturnData ����
	switch (ChosenBuff)
	{
	case ELMBuffType::MoveSpeed:
	{
		// 50% Ȯ���� �� ���� �ӵ� ���� �� �ϳ�
		if (FMath::RandBool())
		{
			ReturnData.MoveSpeedBuffFacor = MoveSpeedBuffFacor;
			ReturnData.Duration = 10.f;
		}
		else
		{
			ReturnData.MoveSpeedBuffFacor = MoveSpeedBuff2Factor;
			ReturnData.Duration = 5.f;
		}
	}
	break;

	case ELMBuffType::Invulnerable:
	{
		ReturnData.bIsVulnaerable = true;
		ReturnData.Duration = 10.f;
	}
	break;

	case ELMBuffType::Heal:
	{
		ReturnData.HealHPValue = HealHPValue;
		// �ʿ��ϴٸ� Duration�� ����
	}
	break;

	default:
		// ����� �� �� ����
		break;
	}

}

void UBuffDebuff::GenerateDebuff()
{
	// 1) ��ȯ ������ �ʱ�ȭ
	ReturnData.Init();

	// 2) �̰� ���� Debuff Ÿ�Ը� ���� �迭 (None ����)
	const ELMDebuffType DebuffOptions[] = {
		ELMDebuffType::Slow,
		ELMDebuffType::HPDecrease
	};
	const int32 NumOptions = UE_ARRAY_COUNT(DebuffOptions);

	// 3) 0 ~ NumOptions-1 ������ ���� �ε���
	int32 ChoiceIndex = FMath::RandRange(0, NumOptions - 1);
	ELMDebuffType ChosenDebuff = DebuffOptions[ChoiceIndex];
	CurrentDebuff = ChosenDebuff;

	// 4) ���õ� Debuff �� ���� ReturnData ����
	switch (ChosenDebuff)
	{
	case ELMDebuffType::Slow:
	{
		// �ӵ� ������� 50% Ȯ���� �� ���� ���� �� �ϳ�
		if (FMath::RandBool())
		{
			ReturnData.MoveSpeedDebuffFactor = MoveSpeedDebuffFactor;
			ReturnData.Duration = 10.f;
		}
		else
		{
			ReturnData.MoveSpeedDebuffFactor = MoveSpeedDebuff2Factor;
			ReturnData.Duration = 5.f;
		}
	}
	break;

	case ELMDebuffType::HPDecrease:
	{
		ReturnData.DecreaseHPValue = DecreaseHPValue;
		// �ʿ��ϴٸ� Duration �� ����
	}
	break;

	default:
		// �迭�� None �� �����Ƿ� ����δ� ���� ���� �ʽ��ϴ�.
		break;
	}
}

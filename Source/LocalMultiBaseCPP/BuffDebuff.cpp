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
	// (1) 사용할 버프 타입을 배열로 선언
	static const ELMBuffType BuffOptions[] = {
		ELMBuffType::MoveSpeed,
		ELMBuffType::Invulnerable,
		ELMBuffType::Heal
	};

	// 버프 반환 구조체 초기화
	ReturnData.Init();

	// (2) 0~2 사이 인덱스를 랜덤하게 뽑고 해당 enum 선택
	int32 ChoiceIndex = FMath::RandRange(0, UE_ARRAY_COUNT(BuffOptions) - 1);
	ELMBuffType ChosenBuff = BuffOptions[ChoiceIndex];
	CurrentBuff = ChosenBuff;

	// (3) 선택된 버프에 따라 ReturnData 세팅
	switch (ChosenBuff)
	{
	case ELMBuffType::MoveSpeed:
	{
		// 50% 확률로 두 가지 속도 버프 중 하나
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
		// 필요하다면 Duration도 설정
	}
	break;

	default:
		// 여기로 올 일 없음
		break;
	}

}

void UBuffDebuff::GenerateDebuff()
{
	// 1) 반환 데이터 초기화
	ReturnData.Init();

	// 2) 뽑고 싶은 Debuff 타입만 담은 배열 (None 제외)
	const ELMDebuffType DebuffOptions[] = {
		ELMDebuffType::Slow,
		ELMDebuffType::HPDecrease
	};
	const int32 NumOptions = UE_ARRAY_COUNT(DebuffOptions);

	// 3) 0 ~ NumOptions-1 사이의 랜덤 인덱스
	int32 ChoiceIndex = FMath::RandRange(0, NumOptions - 1);
	ELMDebuffType ChosenDebuff = DebuffOptions[ChoiceIndex];
	CurrentDebuff = ChosenDebuff;

	// 4) 선택된 Debuff 에 따라 ReturnData 세팅
	switch (ChosenDebuff)
	{
	case ELMDebuffType::Slow:
	{
		// 속도 디버프도 50% 확률로 두 가지 강도 중 하나
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
		// 필요하다면 Duration 도 설정
	}
	break;

	default:
		// 배열에 None 이 없으므로 여기로는 절대 오지 않습니다.
		break;
	}
}

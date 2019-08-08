// Fill out your copyright notice in the Description page of Project Settings.


#include "ZZombieAnimInstance.h"
#include "ConstructorHelpers.h"


UZZombieAnimInstance::UZZombieAnimInstance()
{
	bIsAttacking = false;
}

void UZZombieAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = Cast<AZZombie>(TryGetPawnOwner());
	if (Pawn && Pawn->IsValidLowLevel())
	{
		CurrentState = Pawn->GetZombieState();
	}

}

void UZZombieAnimInstance::AnimNotify_ZombieAttackCheck()
{
	OnAttackCheck.Execute();
}

void UZZombieAnimInstance::AnimNotify_ZombieAttackEndCheck()
{
	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn))
	{
		auto Zombie = Cast<AZZombie>(Pawn);
		if (nullptr == Zombie)
		{
			return;
		}

		Zombie->AttackEnd();
	}
	//SetIsAttacking(false);
}

void UZZombieAnimInstance::AnimNotify_ZombieDieCheck()
{
	ZLOG_S(Warning);
	Montage_Stop(CurrentPlayMontage->BlendOut.GetBlendTime(), CurrentPlayMontage);

}

void UZZombieAnimInstance::SetIsAttacking(bool NewState)
{
	bIsAttacking = NewState;
}

bool UZZombieAnimInstance::IsAttacking() const
{
	return bIsAttacking;
}

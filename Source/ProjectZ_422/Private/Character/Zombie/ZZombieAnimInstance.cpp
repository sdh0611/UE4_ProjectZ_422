// Fill out your copyright notice in the Description page of Project Settings.


#include "ZZombieAnimInstance.h"
#include "ZZombie.h"
#include "ConstructorHelpers.h"


UZZombieAnimInstance::UZZombieAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		MONTAGE_ATTACK(TEXT("AnimMontage'/Game/Animation/Zombie/Anim/Zombie_Attack1_Montage.Zombie_Attack1_Montage'"));
	if (MONTAGE_ATTACK.Succeeded())
	{
		MontageTable.Add(TEXT("Attack"), MONTAGE_ATTACK.Object);
	}
	else
	{
		ZLOG(Error, TEXT("Cannot find montage Attack."));
	}

	bIsAttacking = false;
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

void UZZombieAnimInstance::SetIsAttacking(bool NewState)
{
	bIsAttacking = NewState;
}

bool UZZombieAnimInstance::IsAttacking() const
{
	return bIsAttacking;
}

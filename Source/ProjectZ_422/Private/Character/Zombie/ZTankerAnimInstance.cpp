// Fill out your copyright notice in the Description page of Project Settings.


#include "ZTankerAnimInstance.h"
#include "ZTanker.h"
#include "ZZombieAIController.h"
#include "GameFramework/CharacterMovementComponent.h"

void UZTankerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	//auto Tanker = Cast<AZTanker>(TryGetPawnOwner());
	//if (Tanker)
	//{
	//	//bIsRushCooldown = Tanker->IsRushCooldown();
	//	bIsRushing = Tanker->IsRushCooldown();
	//	//if (bIsRushing)
	//	//{
	//	//	ZLOG_S(Error);
	//	//}
	//}
}

void UZTankerAnimInstance::AnimNotify_ScreamStart()
{
	ZLOG_S(Warning);
	auto Zombie = Cast<AZTanker>(TryGetPawnOwner());
	if (!::IsValid(Zombie))
	{
		return;
	}

	//Zombie->SetCurrentSpeed(0.f);
	Zombie->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	//Zombie->bIsScreaming = true;
}

void UZTankerAnimInstance::AnimNotify_ScreamEnd()
{
	ZLOG_S(Warning);
	auto Zombie = Cast<AZTanker>(TryGetPawnOwner());
	if (!::IsValid(Zombie))
	{
		return;
	}

	//Zombie->bIsScreaming = false;
	Zombie->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	//Zombie->ChangeZombieState(EZombieState::Chase);
	//Zombie->ToggleRush(true);

}

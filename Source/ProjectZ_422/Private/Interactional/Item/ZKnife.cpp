// Fill out your copyright notice in the Description page of Project Settings.


#include "ZKnife.h"
#include "ZCharacter.h"
#include "Engine/World.h"


AZKnife::AZKnife()
{
	WeaponCategory = EWeaponCategory::Knife;

	AttackRange = 50.f;
	AttackRadius = 200.f;
}

void AZKnife::Fire()
{

	Super::Fire();
}

void AZKnife::AttackCheck()
{
	TArray<FHitResult> Hits;

	FVector Dir = ItemOwner->GetActorForwardVector();
	FVector Start = ItemOwner->GetActorLocation();
	FVector End = Start + Dir * AttackRange;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(ItemOwner);
	Params.bReturnPhysicalMaterial = true;
	Params.bTraceComplex = false;

	bool bResult = GetWorld()->SweepMultiByChannel(Hits, Start, End, ItemOwner->GetActorRotation().Quaternion(), 
		PROJECTILE_TRACE, FCollisionShape::MakeSphere(AttackRadius));

	if (bResult)
	{
		for (const auto& Hit : Hits)
		{
			if (Hit.GetActor()->ActorHasTag(TEXT("Enemy")))
			{
				FPointDamageEvent PointDamage;
				PointDamage.Damage = Damage;
				PointDamage.HitInfo = Hit;

				//Hit.GetActor()->TakeDamage(Damage, ,ItemOwner->GetController(), this);
			}
		}

	}


}

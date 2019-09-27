// Fill out your copyright notice in the Description page of Project Settings.


#include "ZShopInteractional.h"
#include "ZCharacter.h"
#include "ZPlayerController.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"


AZShopInteractional::AZShopInteractional()
{
	BodyCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BodyCollision"));
	RootComponent = BodyCollision;

	BodyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(RootComponent);

}

void AZShopInteractional::OnInteraction(AZCharacter * NewCharacter)
{
	auto MyPC = NewCharacter->GetController<AZPlayerController>();
	if (nullptr == MyPC)
	{
		return;
	}

	/* PC내의 shop을 호출하는 코드 작성 */
	MyPC->ServerCheckIsShopOpen();


	Super::OnInteraction(NewCharacter);
}

void AZShopInteractional::OnFocus()
{
	BodyMesh->SetRenderCustomDepth(true);

}

void AZShopInteractional::OnFocusEnd()
{
	BodyMesh->SetRenderCustomDepth(false);

}

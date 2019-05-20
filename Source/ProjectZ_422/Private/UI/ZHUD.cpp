// Fill out your copyright notice in the Description page of Project Settings.


#include "ZHUD.h"
#include "ZPlayerController.h"
#include "ZUserHUD.h"
#include "ConstructorHelpers.h"


AZHUD::AZHUD()
{
	static ConstructorHelpers::FClassFinder<UZUserHUD>
		UI_HUD(TEXT("WidgetBlueprint'/Game/Blueprint/Widget/UI_HUD.UI_HUD_C'"));
	if (UI_HUD.Succeeded())
	{
		UserHUDClass = UI_HUD.Class;
	}
	
	PlayerController = nullptr;
	UserHUD = nullptr;
}

void AZHUD::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<AZPlayerController>(GetOwningPlayerController());
	if (PlayerController)
	{
		UserHUD = CreateWidget<UZUserHUD>(PlayerController, UserHUDClass);
		UserHUD->AddToViewport();
	}

}

UZUserHUD * const AZHUD::GetUserHUD() const
{
	return UserHUD;
}

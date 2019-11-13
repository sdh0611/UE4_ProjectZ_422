// Fill out your copyright notice in the Description page of Project Settings.


#include "ZPlayerController.h"
#include "ZPlayerState.h"
#include "ZHUD.h"
#include "ZPlayerCameraManager.h"
#include "ZCharacter.h"
#include "ZCharacterItemStatusComponent.h"
#include "ZUserHUD.h"
#include "ZShopWidget.h"
#include "ZShop.h"
#include "ZGameMode.h"
#include "ZGameInstance.h"
#include "ZInventoryWidget.h"
#include "ZWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "GameFramework/SpectatorPawn.h"


AZPlayerController::AZPlayerController()
{
	PlayerCameraManagerClass = AZPlayerCameraManager::StaticClass();

}

void AZPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//UWidgetBlueprintLibrary::GetAllWidgetsOfClass();

	if (GetNetMode() == ENetMode::NM_DedicatedServer)
	{
		return;
	}

	if (nullptr == UserHUDClass)
	{
		ZLOG(Error, TEXT("UserHUDClass null."));
		return;
	}
	
	if (IsLocalPlayerController())
	{
		UserHUD = CreateWidget<UZUserHUD>(this, UserHUDClass);
		if (nullptr == UserHUD)
		{
			ZLOG(Error, TEXT("Failed to create UserHUD."));
			return;
		}

		UserHUD->AddToViewport();
	}


	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);

}

void AZPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction(TEXT("ToggleInventory"), IE_Pressed, this, &AZPlayerController::ToggleInventory);

	InputComponent->BindAction(TEXT("ToggleInGameMenu"), IE_Pressed, this, &AZPlayerController::ToggleInGameMenu);
	InputComponent->BindAction(TEXT("RemoveWidgetFromTop"), IE_Pressed, this, &AZPlayerController::RemoveWidgetFromTop);

}

UZUserHUD * const AZPlayerController::GetUserHUD() const
{
	return UserHUD;
}

AZHUD * const AZPlayerController::GetZHUD() const
{
	return Cast<AZHUD>(GetHUD());
}

UZCharacterItemStatusComponent * const AZPlayerController::GetCharacterItemStatusComponent() const
{
	auto Chracter = Cast<AZCharacter>(GetPawn());
	if (Chracter)
	{
		return Chracter->GetItemStatusComponent();
	}

	return nullptr;
}

bool AZPlayerController::ServerCheckIsShopOpen_Validate()
{
	return true;
}

void AZPlayerController::ServerCheckIsShopOpen_Implementation()
{
	auto MyGameMode = GetWorld()->GetAuthGameMode<AZGameMode>();
	if (MyGameMode)
	{
		if (MyGameMode->IsShopOpen())
		{
			ClientOpenShop();
		}
	}

}

bool AZPlayerController::ServerBuy_Validate(int32 BuyItemShopID, int32 Quantity)
{
	return true;
}

void AZPlayerController::ServerBuy_Implementation(int32 BuyItemShopID, int32 Quantity)
{
	auto MyGameMode = GetWorld()->GetAuthGameMode<AZGameMode>();
	if (MyGameMode)
	{
		auto Shop = MyGameMode->GetShop();
		if (Shop && Shop->bIsShopOpen)
		{
			Shop->Buy(this, BuyItemShopID, Quantity);
		}

	}
}

bool AZPlayerController::ServerSell_Validate(int32 SellItemInventoryIndex, int32 Quantity)
{
	if (SellItemInventoryIndex < 0)
	{
		return false;
	}

	return true;
}

void AZPlayerController::ServerSell_Implementation(int32 SellItemInventoryIndex, int32 Quantity)
{
	auto MyGameMode = GetWorld()->GetAuthGameMode<AZGameMode>();
	if (MyGameMode)
	{
		auto Shop = MyGameMode->GetShop();
		if (Shop && Shop->bIsShopOpen)
		{
			Shop->Sell(this, SellItemInventoryIndex, Quantity);
		}

	}

}

bool AZPlayerController::ClientOpenShop_Validate()
{
	return true;
}

void AZPlayerController::ClientOpenShop_Implementation()
{
	/* 로컬에서만 실행. */
	//UKismetSystemLibrary::PrintString(GetWorld(), TEXT("PC OpenShop."));
	if (!IsLocalPlayerController())
	{
		return;
	}

	if (UserHUD)
	{
		if (UserHUD->IsShopWidgetOnScreen())
		{
			//UKismetSystemLibrary::PrintString(GetWorld(), TEXT("PC OpenShop fail."));
			return;
		}

		ConstructShopWidget();
		//Shop->OpenShop(this);
	}

}

bool AZPlayerController::ClientCloseShop_Validate()
{
	return true;
}

void AZPlayerController::ClientCloseShop_Implementation()
{
	//UKismetSystemLibrary::PrintString(GetWorld(), TEXT("PC CloseShop"));
	if (UserHUD)
	{
		//UKismetSystemLibrary::PrintString(GetWorld(), TEXT("PC RemoveShop"));
		UserHUD->RemoveShopWidget();
		//Shop->OpenShop(this);
	}
}

bool AZPlayerController::ClientBloodSplatter_Validate()
{
	return true;
}

void AZPlayerController::ClientBloodSplatter_Implementation()
{
	if (UserHUD)
	{
		UserHUD->CallFadeOutBloodSplatterAnim();
	}
}

bool AZPlayerController::ClientAddPitchAndYaw_Validate(float AddPitch, float AddYaw)
{
	return true;
}

void AZPlayerController::ClientAddPitchAndYaw_Implementation(float AddPitch, float AddYaw)
{
	//ZLOG(Error, TEXT("%.2f, %.2f"), AddPitch, AddYaw);

	if (AddPitch != 0.f)
	{
		AddPitchInput(AddPitch);
	}

	if (AddYaw != 0.f)
	{
		AddYawInput(AddYaw);
	}
}

bool AZPlayerController::ClientDrawEndGameMenu_Validate(bool bWin)
{
	return true;
}

void AZPlayerController::ClientDrawEndGameMenu_Implementation(bool bWin)
{
	ZLOG_S(Error);
	if (!IsLocalPlayerController())
	{
		ZLOG_S(Error);
		return;
	}

	if (!UserHUD)
	{
		ZLOG_S(Error);
		return;
	}

	if (bWin)
	{
		UserHUD->DrawWinMenuWidget();
	}
	else
	{
		UserHUD->DrawDeadMenuWidget();
	}

}

void AZPlayerController::ConstructShopWidget()
{
	if (!IsLocalPlayerController())
	{
		return;
	}

	ZLOG_S(Warning);
	if(nullptr == UserHUD)
	{
		return;
	}

	/* 상점 전체 틀 그림. */
	UserHUD->DrawShopWidget();

	/* 상점 아이템들을 그리기 위한 준비. */
	auto ShopWidget = Cast<UZShopWidget>(UserHUD->GetShopWidget());
	if (nullptr == ShopWidget)
	{
		return;
	}

	auto MyPawn = Cast<AZCharacter>(GetPawn());
	if (nullptr == MyPawn)
	{
		return;
	}

	auto ItemStatusComponent = MyPawn->GetItemStatusComponent();
	if (nullptr == ItemStatusComponent)
	{
		ZLOG(Error, TEXT("ItemStatusComponent not exist.."));
		return;
	}

	UDataTable* ShopItemDataTable = GetGameInstance<UZGameInstance>()->GetShopItemDataTable();
	ShopWidget->ConstructBuyWidget(ShopItemDataTable);

}

void AZPlayerController::AddItemToInventoryWidget(AZItem * const NewItem)
{
	if (IsLocalPlayerController())
	{
		if (::IsValid(NewItem))
		{
			if (UserHUD)
			{
				UserHUD->GetInventoryWidget()->AddItemToInventoryWidget(NewItem);
			}
		}
	}
}

void AZPlayerController::AddItemToWeaponInventoryWidget(AZWeapon * const NewWeapon)
{
	if (IsLocalPlayerController())
	{
		if (::IsValid(NewWeapon))
		{
			if (UserHUD)
			{
				UserHUD->GetInventoryWidget()->AddItemToWeaponInventory(NewWeapon);
			}
		}
	}
}

void AZPlayerController::AddItemToSellWidget(AZItem * const NewItem)
{
	if (IsLocalPlayerController())
	{
		if (::IsValid(NewItem))
		{
			if (UserHUD)
			{
				UserHUD->GetShopWidget()->AddItemToSellWidget(NewItem);
			}
		}
	}
}

void AZPlayerController::FadeBloodSplatter()
{
	if (IsLocalPlayerController() && UserHUD)
	{
		UserHUD->CallFadeOutBloodSplatterAnim();
	}


}

void AZPlayerController::ToggleInventory()
{
	if (GetUserHUD())
	{
		GetUserHUD()->ToggleInventoryWidget();
	}

}

void AZPlayerController::ToggleInGameMenu()
{
	if (GetUserHUD())
	{
		GetUserHUD()->ToggleInGameMenuWIdget();
	}

}

void AZPlayerController::RemoveWidgetFromTop()
{
	if (GetUserHUD())
	{
		GetUserHUD()->RemoveWidgetFromTop();
	}

}

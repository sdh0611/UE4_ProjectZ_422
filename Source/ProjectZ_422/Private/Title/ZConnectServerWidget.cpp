// Fill out your copyright notice in the Description page of Project Settings.


#include "ZConnectServerWidget.h"
#include "..\..\Public\Title\ZConnectServerWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

void UZConnectServerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InputIP = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("ETB_InputIP")));
	check(InputIP);

	MakeServerButton = Cast<UButton>(GetWidgetFromName(TEXT("BTN_MakeServer")));
	check(MakeServerButton);

	ConnectServerButton = Cast<UButton>(GetWidgetFromName(TEXT("BTN_ConnectServer")));
	check(ConnectServerButton);

	MakeServerButton->OnClicked.AddDynamic(this, &UZConnectServerWidget::OnMakeServerButtonClick);
	ConnectServerButton->OnClicked.AddDynamic(this, &UZConnectServerWidget::OnConnectServerButtonClick);
}

void UZConnectServerWidget::OnMakeServerButtonClick()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Lobby"), true, TEXT("listen"));
}

void UZConnectServerWidget::OnConnectServerButtonClick()
{
	auto IP = InputIP->GetText();
	if (IP.IsEmpty())
	{
		return;
	}	

	UGameplayStatics::OpenLevel(GetWorld(), *IP.ToString());
	//if (GetWorld()->ServerTravel(IP.ToString()))
	//{
	//	ZLOG(Error, TEXT("Invalid server ip.."));
	//	return;
	//}

}

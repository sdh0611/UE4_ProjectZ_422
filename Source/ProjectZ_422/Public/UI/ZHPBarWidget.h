// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Blueprint/UserWidget.h"
#include "ZHPBarWidget.generated.h"


/**
 * 
 */
UCLASS()
class PROJECTZ_422_API UZHPBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void BindStatus(class UZCharacterStatusComponent* NewStatusComponent);

protected:
	void InterpGage(float DeltaTime);

	void SetUpdateBar();

	UFUNCTION(BlueprintImplementableEvent, Category = HPBar)
	void UpdateWidget();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HPBar)
	TWeakObjectPtr<class UZPlayerStatusComponent> StatusComponent;

protected:
	bool bCanUpdateBar = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float TargetHP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float TargetHPRatio;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float TargetDopingGage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float TargetDopingGageRatio;

};

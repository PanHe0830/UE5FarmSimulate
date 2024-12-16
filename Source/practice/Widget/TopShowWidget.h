// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TopShowWidget.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICE_API UTopShowWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ShowText;

	void SetWidgetText(FString Name);
};

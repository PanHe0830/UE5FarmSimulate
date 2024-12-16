// Fill out your copyright notice in the Description page of Project Settings.


#include "TopShowWidget.h"
#include "Components/TextBlock.h"

void UTopShowWidget::SetWidgetText(FString Name)
{
	ShowText->SetText(FText::FromString(Name));
}

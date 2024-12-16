// Fill out your copyright notice in the Description page of Project Settings.

/** 
*	20241216 当前类作为角色类的 "装备" 的逻辑 包括 角色工具装备 工具的动作(挥 砍 等) 工具的耐久
*/

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ToolComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PRACTICE_API UToolComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UToolComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY()
	class APracticeCharacter* Character;

	UPROPERTY(EditAnywhere, Category = "ToolProperty")
	int32 ToolHealth = 100; // 工具的耐久

public:
	// 设置当前是哪个角色的工具组件
	void SetCurrentCharacter(class APracticeCharacter* pointer);

	// 装备工具
	void EquipTool(class AToolActor* Actor);

	// 抛弃工具
	void DropTool();

	UPROPERTY()
	class AToolActor* CurrentTool;

private:
	void AttachActorToRightHand(AActor* ActorAttach);
};

// Fill out your copyright notice in the Description page of Project Settings.

/** 
*	20241216 ��ǰ����Ϊ��ɫ��� "װ��" ���߼� ���� ��ɫ����װ�� ���ߵĶ���(�� �� ��) ���ߵ��;�
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
	int32 ToolHealth = 100; // ���ߵ��;�

public:
	// ���õ�ǰ���ĸ���ɫ�Ĺ������
	void SetCurrentCharacter(class APracticeCharacter* pointer);

	// װ������
	void EquipTool(class AToolActor* Actor);

	// ��������
	void DropTool();

	UPROPERTY()
	class AToolActor* CurrentTool;

private:
	void AttachActorToRightHand(AActor* ActorAttach);
};

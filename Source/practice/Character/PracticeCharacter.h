// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PracticeCharacter.generated.h"

UCLASS()
class PRACTICE_API APracticeCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APracticeCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// ��ɫ��ǰ�ƶ�����
	virtual void CharacterMoveForward(float Value);
	// ��ɫ�����ƶ�����
	virtual void CharacterMoveRight(float Value);
	// ������ƶ�����
	virtual void CameraTurn(float Value);
	// ������ƶ�����
	virtual void CameraLookUp(float Value);
	// ��ɫ��Ծ����
	virtual void Jump() override;
	// ��ɫ����˺�����
	virtual void CauseDamage();
	// ��ɫʰȡװ������
	virtual void CharacterEquip();
	// ��ɫ�ӵ�����
	void CharacterDropTool();

public:
	void SetToolActorPointer(class AToolActor* Tool);

private:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class UCameraComponent* FollowCamera;

	UPROPERTY()
	class UToolComponent* ToolComponent;

	UPROPERTY()
	class AToolActor* ToolActor;
};

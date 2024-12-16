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
	// 角色向前移动函数
	virtual void CharacterMoveForward(float Value);
	// 角色向右移动函数
	virtual void CharacterMoveRight(float Value);
	// 摄像机移动左右
	virtual void CameraTurn(float Value);
	// 摄像机移动上下
	virtual void CameraLookUp(float Value);
	// 角色跳跃函数
	virtual void Jump() override;
	// 角色造成伤害函数
	virtual void CauseDamage();
	// 角色拾取装备函数
	virtual void CharacterEquip();
	// 角色扔掉工具
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

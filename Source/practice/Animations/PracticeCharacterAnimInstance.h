// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PracticeCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICE_API UPracticeCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	/* ��ʼ������ */
	virtual void NativeInitializeAnimation() override;
	/* ���¶���  ������֡�� */
	virtual void NativeUpdateAnimation(float DeltaTime)override;

private:
	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))/** ��ͼֻ�� ������Character ����˽�з��� */
	class APracticeCharacter* Character;

	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	float Speed;

	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	float Direction;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float YawOffset;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float Lean;

#if 0
	FRotator CharacterRotationLastFrame;
	FRotator CharacterRotation;
	FRotator DeltaRotation;
#endif
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "PracticeCharacterAnimInstance.h"
#include "practice/Character/PracticeCharacter.h"
#include "Kismet/KismetMathLibrary.h"

void UPracticeCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Character = Cast<APracticeCharacter>(TryGetPawnOwner());
}

void UPracticeCharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (Character == nullptr)
	{
		Character = Cast<APracticeCharacter>(TryGetPawnOwner());
	}

	if (Character == nullptr) return;

	FVector CharacterVec = Character->GetVelocity();
	CharacterVec.Z = 0.f;
	Speed = CharacterVec.Size();

	FVector vec = Character->GetVelocity();
	FRotator rotator = Character->GetActorRotation();
	Direction = CalculateDirection(vec,rotator);

#if 0
	// Offset Yaw for Strafing
	FRotator AimRotation = Character->GetBaseAimRotation();
	FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(Character->GetVelocity());
	FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation);
	DeltaRotation = FMath::RInterpTo(DeltaRotation, DeltaRot, DeltaTime, 6.f);
	Speed = DeltaRotation.Yaw;

	CharacterRotationLastFrame = CharacterRotation;
	CharacterRotation = Character->GetActorRotation();
	const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, CharacterRotationLastFrame);
	const float Target = Delta.Yaw / DeltaTime;
	const float Interp = FMath::FInterpTo(Lean, Target, DeltaTime, 6.f);
	Direction = FMath::Clamp(Interp, -90.f, 90.f);
#endif
}

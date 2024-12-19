// Fill out your copyright notice in the Description page of Project Settings.


#include "PracticeCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "practice/Plant/PlantActor.h"
#include "practice/Component/ToolComponent.h"
#include "practice/Tool/ToolActor.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"

// Sets default values
APracticeCharacter::APracticeCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("DefaultCameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = 600.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("DefaultFollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom);
	FollowCamera->bUsePawnControlRotation = false;

	ToolComponent = CreateDefaultSubobject<UToolComponent>(TEXT("ToolComponent"));
}

// Called when the game starts or when spawned
void APracticeCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	ToolComponent->SetCurrentCharacter(this);
}

// Called every frame
void APracticeCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APracticeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward",this,&APracticeCharacter::CharacterMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APracticeCharacter::CharacterMoveRight); 
	PlayerInputComponent->BindAxis("Turn", this, &APracticeCharacter::CameraTurn);
	PlayerInputComponent->BindAxis("LookUp", this, &APracticeCharacter::CameraLookUp); 

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APracticeCharacter::Jump); 
	PlayerInputComponent->BindAction("Damage", IE_Pressed, this, &APracticeCharacter::CauseDamage);
	PlayerInputComponent->BindAction("Equip", IE_Pressed, this, &APracticeCharacter::CharacterEquip); 
	PlayerInputComponent->BindAction("Drop", IE_Pressed, this, &APracticeCharacter::CharacterDropTool); 
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APracticeCharacter::CharacterAttack);
}

void APracticeCharacter::CharacterMoveForward(float Value)
{
	if (Controller != nullptr && Value != 0.f)
	{
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));
		AddMovementInput(Direction, Value);
	}
}

void APracticeCharacter::CharacterMoveRight(float Value)
{
	if (Controller != nullptr && Value != 0.f)
	{
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y));
		AddMovementInput(Direction, Value);
	}
}

void APracticeCharacter::CameraTurn(float Value)
{
	AddControllerYawInput(Value);
}

void APracticeCharacter::CameraLookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void APracticeCharacter::Jump()
{
	Super::Jump();
}

void APracticeCharacter::CauseDamage()
{
	//UGameplayStatics::ApplyDamage(this, 10, Controller, TreeActor, UDamageType::StaticClass());
}

void APracticeCharacter::CharacterEquip()
{
	// 角色装备工具
	ToolComponent->EquipTool(ToolActor);
}

void APracticeCharacter::CharacterDropTool()
{
	if (ToolActor)
	{
		ToolActor->DropTool();

		SetToolActorPointer(nullptr);
	}
}

void APracticeCharacter::CharacterAttack()
{
	if (ToolActor &&ToolComponent)
	{
		ToolComponent->HandleAttack();
	}
}

void APracticeCharacter::SetToolActorPointer(AToolActor* Tool)
{
	ToolActor = Tool;
}

void APracticeCharacter::PlayAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance->Montage_IsPlaying(AttackMontage))
	{
		UE_LOG(LogTemp, Warning, TEXT("Attack Montage is already playing!"));
		return;
	}

	if (AttackMontage && AnimInstance)
	{
		AnimInstance->Montage_Play(AttackMontage, 1.0f);
	}
	
}


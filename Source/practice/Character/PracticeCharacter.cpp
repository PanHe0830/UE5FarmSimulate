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
#include "Components/BoxComponent.h"
#include "practice/practice.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

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

void APracticeCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void APracticeCharacter::Look(const FInputActionValue& Value)
{	
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
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

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APracticeCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APracticeCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}

#if 0
	PlayerInputComponent->BindAxis("MoveForward",this,&APracticeCharacter::CharacterMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APracticeCharacter::CharacterMoveRight); 
	PlayerInputComponent->BindAxis("Turn", this, &APracticeCharacter::CameraTurn);
	PlayerInputComponent->BindAxis("LookUp", this, &APracticeCharacter::CameraLookUp); 

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APracticeCharacter::Jump); 
#endif
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


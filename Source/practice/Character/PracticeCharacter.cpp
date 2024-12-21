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

	RightHandBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	RightHandBox->SetupAttachment(GetMesh());
	RightHandBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // 启用碰撞
	RightHandBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic); // 设置为动态物体
	RightHandBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore); // 忽略所有默认通道
	RightHandBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Block); // 与树木（物理物体）发生碰撞
	RightHandBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	RightHandBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);

	BodyBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Body"));
	BodyBox->SetupAttachment(GetMesh());
	BodyBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // 启用碰撞
	BodyBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic); // 设置为动态物体
	BodyBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore); // 忽略所有默认通道
	BodyBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Block); // 与树木（物理物体）发生碰撞
	BodyBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	BodyBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);

	BodyBox->GetGenerateOverlapEvents();
	BodyBox->CanEverAffectNavigation();
}

// Called when the game starts or when spawned
void APracticeCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	ToolComponent->SetCurrentCharacter(this);
	RightHandBox->OnComponentHit.AddDynamic(this,&APracticeCharacter::OnHitHand);

	BodyBox->OnComponentHit.AddDynamic(this, &APracticeCharacter::OnHitBody);
}

void APracticeCharacter::OnHitHand(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("OnHitHand"));
}

void APracticeCharacter::OnHitBody(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("OnHitBody"));
}

// Called every frame
void APracticeCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (ToolActor)
	//{
	//	ToolActor->SetCurrentBoxLocation(this);
	//}
	FVector BoxOrigin = RightHandBox->GetComponentLocation();
	FVector BoxExtent = RightHandBox->GetScaledBoxExtent(); // 获取碰撞盒的尺寸
	
	// 画出调试框
	DrawDebugBox(GetWorld(), BoxOrigin, BoxExtent, FQuat::Identity, FColor::Green, false, -1.0f, 0, 2.0f);

	FVector BoxOriginb = BodyBox->GetComponentLocation();
	FVector BoxExtentb = BodyBox->GetScaledBoxExtent(); // 获取碰撞盒的尺寸

	// 画出调试框
	DrawDebugBox(GetWorld(), BoxOriginb, BoxExtentb, FQuat::Identity, FColor::Green, false, -1.0f, 0, 2.0f);
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


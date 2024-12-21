// Fill out your copyright notice in the Description page of Project Settings.


#include "ToolActor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "practice/Widget/TopShowWidget.h"
#include "Components/SphereComponent.h"
#include "practice/Character/PracticeCharacter.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AToolActor::AToolActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	KnifeComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Knife"));
	KnifeComponent->SetupAttachment(RootComponent);
	KnifeComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); //KnifeHand

	HeadShowWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HeadShowWidget"));
	HeadShowWidget->SetupAttachment(KnifeComponent);

	AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));
	AreaSphere->SetupAttachment(KnifeComponent);
	AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	CollisionBox->SetupAttachment(KnifeComponent,FName("Blade_end"));
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore); // 忽略所有默认通道
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore); // 与树木（物理物体）发生碰撞
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
}

// Called when the game starts or when spawned
void AToolActor::BeginPlay()
{
	Super::BeginPlay();
	
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	AreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &AToolActor::OnSphereStartOverlap);
	AreaSphere->OnComponentEndOverlap.AddDynamic(this, &AToolActor::OnSphereEndOverlap);

	SetHeadShow(false);

	if (CollisionBox)
	{
		CollisionBox->OnComponentHit.AddDynamic(this, &AToolActor::OnHit);
	}
}

// Called every frame
void AToolActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DrawDebugBoxx();
}

void AToolActor::DrawDebugBoxx()
{
	FVector BoxOrigin = CollisionBox->GetComponentLocation();
	FVector BoxExtent = CollisionBox->GetScaledBoxExtent(); // 获取碰撞盒的尺寸

	// 画出调试框
	DrawDebugBox(GetWorld(), BoxOrigin, BoxExtent, FQuat::Identity, FColor::Green, false, -1.0f, 0, 2.0f);
}

void AToolActor::DropTool()
{
	FDetachmentTransformRules DetachResult(EDetachmentRule::KeepWorld,true);
	KnifeComponent->DetachFromComponent(DetachResult);
	SetOwner(nullptr);

	bEquipFlag = false;
}

void AToolActor::OnSphereStartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	SetHeadShow(true);
	SetToolTopText();

	APracticeCharacter* Character = Cast<APracticeCharacter>(OtherActor);
	if (Character)
	{
		Character->SetToolActorPointer(this);
	}

}

void AToolActor::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	SetHeadShow(false);

	//APracticeCharacter* Character = Cast<APracticeCharacter>(OtherActor);
	//if (Character)
	//{
	//	Character->SetToolActorPointer(nullptr);
	//}
}

void AToolActor::SetHeadShow(bool retFlag)
{
	if (HeadShowWidget)
	{
		HeadShowWidget->SetVisibility(retFlag);
	}
}

void AToolActor::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("Tool OnHit"));
#if 0
	APracticeCharacter* Character = Cast<APracticeCharacter>(OtherActor);
	if (Character)
	{
		AController* CharacterController = Cast<AController>(Character->Controller);
		if (CharacterController)
		{
			UGameplayStatics::ApplyDamage(OtherActor, Damage, CharacterController, this, UDamageType::StaticClass());
		}
	}
#endif
}

void AToolActor::SetToolTopText()
{
	if (HeadShowWidget == nullptr) return;
	UTopShowWidget* temp = Cast<UTopShowWidget>(HeadShowWidget->GetUserWidgetObject());
	if (temp)
	{
		temp->SetWidgetText("Knife");
	}
}


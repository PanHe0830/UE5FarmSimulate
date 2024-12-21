// Fill out your copyright notice in the Description page of Project Settings.


#include "PlantActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "practice/Character/PracticeCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "practice/practice.h"

// Sets default values
APlantActor::APlantActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TreeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tree"));
	TreeMesh->SetupAttachment(RootComponent);
	TreeMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	TreeMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	TreeMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	TreeMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
	TreeMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	CollisionBox->SetupAttachment(TreeMesh);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	CollisionBox->SetMobility(EComponentMobility::Movable);
}

// Called when the game starts or when spawned
void APlantActor::BeginPlay()
{
	Super::BeginPlay();	
	
	OnTakeAnyDamage.AddDynamic(this,&APlantActor::Damage);
	CollisionBox->OnComponentHit.AddDynamic(this,&APlantActor::OnHit);
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this,&APlantActor::Begin);
}

// Called every frame
void APlantActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DrawDebugBoxx();
}

void APlantActor::DrawDebugBoxx()
{
	FVector BoxOrigin = CollisionBox->GetComponentLocation();
	FVector BoxExtent = CollisionBox->GetScaledBoxExtent(); // 获取碰撞盒的尺寸

	// 画出调试框
	DrawDebugBox(GetWorld(), BoxOrigin, BoxExtent, FQuat::Identity, FColor::Green, false, -1.0f, 0, 2.0f);
}

void APlantActor::Damage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	//UE_LOG(LogTemp, Warning, TEXT("Damage"));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("树木受到了伤害"));
}

void APlantActor::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	APracticeCharacter* Character = Cast<APracticeCharacter>(OtherActor);
	if (Character)
	{	
		AController* CharacterController = Cast<AController>(Character->Controller);
		if (CharacterController)
		{
			UGameplayStatics::ApplyDamage(OtherActor, 10, CharacterController, this, UDamageType::StaticClass());
		}
	}

}

void APlantActor::Begin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Plant Begin"));
}

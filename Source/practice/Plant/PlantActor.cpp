// Fill out your copyright notice in the Description page of Project Settings.


#include "PlantActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "practice/Character/PracticeCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APlantActor::APlantActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TreeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tree"));
	TreeMesh->SetupAttachment(RootComponent);
	TreeMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	CollisionBox->SetupAttachment(TreeMesh);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // ������ײ
	CollisionBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic); // ����Ϊ��̬���壨��ľͨ���Ǿ�̬�ģ�
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore); // Ĭ�Ϻ�������ͨ��
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
	//CollisionBox->SetSimulatePhysics(true);
}

// Called when the game starts or when spawned
void APlantActor::BeginPlay()
{
	Super::BeginPlay();	
	
	OnTakeAnyDamage.AddDynamic(this,&APlantActor::Damage);
	CollisionBox->OnComponentHit.AddDynamic(this,&APlantActor::OnHit);
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
	FVector BoxExtent = CollisionBox->GetScaledBoxExtent(); // ��ȡ��ײ�еĳߴ�

	// �������Կ�
	DrawDebugBox(GetWorld(), BoxOrigin, BoxExtent, FQuat::Identity, FColor::Green, false, -1.0f, 0, 2.0f);
}

void APlantActor::Damage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	//UE_LOG(LogTemp, Warning, TEXT("Damage"));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("��ľ�ܵ����˺�"));
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
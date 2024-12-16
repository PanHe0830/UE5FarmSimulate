// Fill out your copyright notice in the Description page of Project Settings.


#include "ToolActor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "practice/Widget/TopShowWidget.h"
#include "Components/SphereComponent.h"
#include "practice/Character/PracticeCharacter.h"

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
}

// Called when the game starts or when spawned
void AToolActor::BeginPlay()
{
	Super::BeginPlay();
	
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	AreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &AToolActor::OnSphereOverlap);
	AreaSphere->OnComponentEndOverlap.AddDynamic(this, &AToolActor::OnSphereEndOverlap);

	SetHeadShow(false);
}

// Called every frame
void AToolActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AToolActor::DropTool()
{
	FDetachmentTransformRules DetachResult(EDetachmentRule::KeepWorld,true);
	KnifeComponent->DetachFromComponent(DetachResult);
	SetOwner(nullptr);
}

void AToolActor::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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

	APracticeCharacter* Character = Cast<APracticeCharacter>(OtherActor);
	if (Character)
	{
		Character->SetToolActorPointer(nullptr);
	}
}

void AToolActor::SetHeadShow(bool retFlag)
{
	if (HeadShowWidget)
	{
		HeadShowWidget->SetVisibility(retFlag);
	}
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


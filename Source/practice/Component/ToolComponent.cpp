// Fill out your copyright notice in the Description page of Project Settings.


#include "ToolComponent.h"
#include "practice/Character/PracticeCharacter.h"
#include "practice/Tool/ToolActor.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"

// Sets default values for this component's properties
UToolComponent::UToolComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UToolComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// Called every frame
void UToolComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UToolComponent::SetCurrentCharacter(APracticeCharacter* pointer)
{
	Character = pointer;
}

void UToolComponent::EquipTool(AToolActor* Actor)
{
	if (Actor == nullptr) return;
	DropTool();
	CurrentTool = Actor;
	AttachActorToRightHand(CurrentTool);
	CurrentTool->SetOwner(Character);
	//USkeletalMeshComponent* Mesh = CurrentTool->GeTToolMeshComponent();
	USphereComponent* Sphere = CurrentTool->GeTToolSphereComponent();
	if (Sphere)
	{
		Sphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	}

	CurrentTool->bEquipFlag = true;
}

void UToolComponent::DropTool()
{
	if (CurrentTool)
	{
		CurrentTool->DropTool();
		USphereComponent* Sphere = CurrentTool->GeTToolSphereComponent();
		if (Sphere)
		{
			Sphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
		}

		CurrentTool->bEquipFlag = true;
	}
}

void UToolComponent::AttachActorToRightHand(AActor* ActorAttach)
{
	if (Character == nullptr || Character->GetMesh() == nullptr || ActorAttach == nullptr) return;
	const USkeletalMeshSocket* HandSocket = Character->GetMesh()->GetSocketByName(FName("KnifeHand"));
	if (HandSocket)
	{
		HandSocket->AttachActor(ActorAttach, Character->GetMesh());
	}
}


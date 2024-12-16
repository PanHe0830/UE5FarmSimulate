// Fill out your copyright notice in the Description page of Project Settings.


#include "RandomTreeSpawner.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Engine/StaticMeshActor.h"
#include "EngineUtils.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

// Sets default values
ARandomTreeSpawner::ARandomTreeSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ����ʵ������̬���������
	InstancedMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedMeshComponent"));
	RootComponent = InstancedMeshComponent;
}

// Called when the game starts or when spawned
void ARandomTreeSpawner::BeginPlay()
{
	Super::BeginPlay();

    if (TreeMesh)
    {
        FString FloorName = TEXT("Floor");
        GetStaticMeshTerrainSize(GetWorld(), FloorName);
        InstancedMeshComponent->SetStaticMesh(TreeMesh);
        GenerateTrees();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("TreeMesh is not set! Please assign a static mesh."));
    }
}

// Called every frame
void ARandomTreeSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARandomTreeSpawner::GenerateTrees()
{
    for (int32 i = 0; i < NumberOfTrees; i++)
    {
        FVector Center = (SpawnAreaMin + SpawnAreaMax) / 2;
        UE_LOG(LogTemp, Log, TEXT("Center: %s"), *Center.ToString());
        FVector Extents = (SpawnAreaMax - SpawnAreaMin) / 2;
        UE_LOG(LogTemp, Log, TEXT("Extents: %s"), *Extents.ToString());

        // �������λ��
        FVector RandomLocation = UKismetMathLibrary::RandomPointInBoundingBox(
            Center,
            Extents
        );

        // ����Zֵ��ȷ����ľ���ϵ���
        FHitResult HitResult;
        FCollisionQueryParams CollisionParams;
        FVector StartLocation = RandomLocation + FVector(0.f, 0.f, 0.f);
        FVector EndLocation = RandomLocation - FVector(0.f, 0.f, 0.f);
        
        // ���е��θ߶ȼ��
        if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, CollisionParams))
        {
            RandomLocation.Z = HitResult.Location.Z;
        }

        // �����ת
        FRotator RandomRotation = FRotator(0.f, FMath::RandRange(0.f, 360.f), 0.f);

        // �������
        FVector RandomScale = FVector(
            FMath::FRandRange(0.8f, 1.2f),
            FMath::FRandRange(0.8f, 1.2f),
            FMath::FRandRange(0.8f, 1.2f)
        );

        // ����ʵ���任
        FTransform InstanceTransform(RandomRotation, RandomLocation, RandomScale);
        //UE_LOG(LogTemp, Log, TEXT("RandomLocation: %s"), *RandomLocation.ToString());

        //DrawDebugSphere(GetWorld(), RandomLocation, 100.f, 12, FColor::Red, true);

        // ���ʵ��
        InstancedMeshComponent->AddInstance(InstanceTransform);
    }
}

FVector ARandomTreeSpawner::GetStaticMeshTerrainSize(UWorld* World , FString& ActorName)
{
    if (!World)
    {
        return FVector::ZeroVector;
    }

    for (TActorIterator<AStaticMeshActor> It(World); It; ++It)
    {
        AStaticMeshActor* StaticMeshActor = *It;
        if (StaticMeshActor && StaticMeshActor->GetStaticMeshComponent() && StaticMeshActor->GetName() == ActorName)
        {
            UStaticMeshComponent* MeshComponent = StaticMeshActor->GetStaticMeshComponent();
            UStaticMesh* StaticMesh = MeshComponent->GetStaticMesh();
            //FVector worldposition = MeshComponent->GetComponentLocation(); // �����������
            //UE_LOG(LogTemp, Log, TEXT("worldposition: %s"), *worldposition.ToString());
            //DrawDebugSphere(GetWorld(), worldposition, 100.f, 12, FColor::Red, true);
            if (StaticMesh)
            {
                // ��ȡ���ذ�Χ��
                FBox LocalBoundingBox = StaticMesh->GetBoundingBox();

                // ת������������
                FVector WorldMin = StaticMeshActor->GetActorTransform().TransformPosition(LocalBoundingBox.Min);
                FVector WorldMax = StaticMeshActor->GetActorTransform().TransformPosition(LocalBoundingBox.Max);
                
                // ������������
                SpawnAreaMin = WorldMin;
                SpawnAreaMax = WorldMax;

                // ��ӡ��־��֤
                UE_LOG(LogTemp, Log, TEXT("SpawnAreaMin: %s, SpawnAreaMax: %s"), *SpawnAreaMin.ToString(), *SpawnAreaMax.ToString());

                return WorldMax - WorldMin; // �������緶Χ��С
            }
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("No StaticMeshActor found in the current world."));
    return FVector::ZeroVector;
}
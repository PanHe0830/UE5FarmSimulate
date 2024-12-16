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

	// 创建实例化静态网格体组件
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

        // 随机生成位置
        FVector RandomLocation = UKismetMathLibrary::RandomPointInBoundingBox(
            Center,
            Extents
        );

        // 调整Z值，确保树木贴合地形
        FHitResult HitResult;
        FCollisionQueryParams CollisionParams;
        FVector StartLocation = RandomLocation + FVector(0.f, 0.f, 0.f);
        FVector EndLocation = RandomLocation - FVector(0.f, 0.f, 0.f);
        
        // 进行地形高度检测
        if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, CollisionParams))
        {
            RandomLocation.Z = HitResult.Location.Z;
        }

        // 随机旋转
        FRotator RandomRotation = FRotator(0.f, FMath::RandRange(0.f, 360.f), 0.f);

        // 随机缩放
        FVector RandomScale = FVector(
            FMath::FRandRange(0.8f, 1.2f),
            FMath::FRandRange(0.8f, 1.2f),
            FMath::FRandRange(0.8f, 1.2f)
        );

        // 创建实例变换
        FTransform InstanceTransform(RandomRotation, RandomLocation, RandomScale);
        //UE_LOG(LogTemp, Log, TEXT("RandomLocation: %s"), *RandomLocation.ToString());

        //DrawDebugSphere(GetWorld(), RandomLocation, 100.f, 12, FColor::Red, true);

        // 添加实例
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
            //FVector worldposition = MeshComponent->GetComponentLocation(); // 获得世界坐标
            //UE_LOG(LogTemp, Log, TEXT("worldposition: %s"), *worldposition.ToString());
            //DrawDebugSphere(GetWorld(), worldposition, 100.f, 12, FColor::Red, true);
            if (StaticMesh)
            {
                // 获取本地包围盒
                FBox LocalBoundingBox = StaticMesh->GetBoundingBox();

                // 转换到世界坐标
                FVector WorldMin = StaticMeshActor->GetActorTransform().TransformPosition(LocalBoundingBox.Min);
                FVector WorldMax = StaticMeshActor->GetActorTransform().TransformPosition(LocalBoundingBox.Max);
                
                // 设置生成区域
                SpawnAreaMin = WorldMin;
                SpawnAreaMax = WorldMax;

                // 打印日志验证
                UE_LOG(LogTemp, Log, TEXT("SpawnAreaMin: %s, SpawnAreaMax: %s"), *SpawnAreaMin.ToString(), *SpawnAreaMax.ToString());

                return WorldMax - WorldMin; // 返回世界范围大小
            }
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("No StaticMeshActor found in the current world."));
    return FVector::ZeroVector;
}
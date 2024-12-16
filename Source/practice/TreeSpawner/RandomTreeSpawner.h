// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RandomTreeSpawner.generated.h"

UCLASS()
class PRACTICE_API ARandomTreeSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARandomTreeSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
    // 树木静态网格体
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TreeSpawner")
    class UStaticMesh* TreeMesh;

    // 生成区域的边界
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TreeSpawner")
    FVector SpawnAreaMin = FVector(-5000.f, -5000.f, 0.f); // 最小边界
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TreeSpawner")
    FVector SpawnAreaMax = FVector(5000.f, 5000.f, 0.f);  // 最大边界

    // 树木生成数量
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TreeSpawner")
    int32 NumberOfTrees = 10;

    // 实例化网格组件
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TreeSpawner")
    class UInstancedStaticMeshComponent* InstancedMeshComponent;

private:
    // 生成树木的方法
    void GenerateTrees();
    FVector GetStaticMeshTerrainSize(UWorld* World, FString& ActorName);
};

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
    // ��ľ��̬������
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TreeSpawner")
    class UStaticMesh* TreeMesh;

    // ��������ı߽�
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TreeSpawner")
    FVector SpawnAreaMin = FVector(-5000.f, -5000.f, 0.f); // ��С�߽�
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TreeSpawner")
    FVector SpawnAreaMax = FVector(5000.f, 5000.f, 0.f);  // ���߽�

    // ��ľ��������
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TreeSpawner")
    int32 NumberOfTrees = 10;

    // ʵ�����������
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TreeSpawner")
    class UInstancedStaticMeshComponent* InstancedMeshComponent;

private:
    // ������ľ�ķ���
    void GenerateTrees();
    FVector GetStaticMeshTerrainSize(UWorld* World, FString& ActorName);
};

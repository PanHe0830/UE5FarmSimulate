// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlantActor.generated.h"

UCLASS()
class PRACTICE_API APlantActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlantActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void DrawDebugBoxx();

public:
	// 树木静态网格体
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor")
	class UStaticMeshComponent* TreeMesh;

private:
	// 收到的伤害
	UFUNCTION()
	virtual void Damage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// 生成的树的血量
	UPROPERTY(EditAnywhere, Category = PROPERTY)
	float TreeHealth = 100.f;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* CollisionBox;
};

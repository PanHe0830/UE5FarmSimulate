// Fill out your copyright notice in the Description page of Project Settings.

// ��ǰ���ǹ�����

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ToolActor.generated.h"

UCLASS()
class PRACTICE_API AToolActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AToolActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void DrawDebugBoxx();

	// ������ǰ����
	void DropTool();

	//void SetCurrentBoxLocation(class APracticeCharacter* Pointer);

protected:
	UFUNCTION()
	virtual void OnSphereStartOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void OnSphereEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

	void SetHeadShow(bool retFlag);

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnKnifeBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, Category = "Component")
	class USkeletalMeshComponent* KnifeComponent;

	UPROPERTY(EditAnywhere, Category = "Widget")
	class UWidgetComponent* HeadShowWidget;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	class USphereComponent* AreaSphere;

	void SetToolTopText();

	UPROPERTY(EditAnywhere)
	class UBoxComponent* CollisionBox;

	UPROPERTY(EditAnywhere)
	int32 Damage = 50;

public:
	UPROPERTY(VisibleAnywhere , Category = "Component")
	bool bEquipFlag = false; // ��ǰ�����Ƿ�װ��

public:
	FORCEINLINE USkeletalMeshComponent* GeTToolMeshComponent() { return KnifeComponent; };
	FORCEINLINE USphereComponent* GeTToolSphereComponent() { return AreaSphere; };
};

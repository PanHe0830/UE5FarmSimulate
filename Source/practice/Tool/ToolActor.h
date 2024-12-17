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

	// ������ǰ����
	void DropTool();

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


	UPROPERTY(EditAnywhere, Category = "Component")
	class USkeletalMeshComponent* KnifeComponent;

	UPROPERTY(EditAnywhere, Category = "Widget")
	class UWidgetComponent* HeadShowWidget;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	class USphereComponent* AreaSphere;

	void SetToolTopText();

public:
	UPROPERTY(VisibleAnywhere , Category = "Component")
	bool bEquipFlag = false; // ��ǰ�����Ƿ�װ��

public:
	FORCEINLINE USkeletalMeshComponent* GeTToolMeshComponent() { return KnifeComponent; };
	FORCEINLINE USphereComponent* GeTToolSphereComponent() { return AreaSphere; };
};

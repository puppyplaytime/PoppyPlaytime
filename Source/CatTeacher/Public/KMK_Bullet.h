// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KMK_Bullet.generated.h"

UCLASS()
class CATTEACHER_API AKMK_Bullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKMK_Bullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Die();
	// �浹ü, �ܰ��� �����ʹ�.
	UPROPERTY(EditDefaultsOnly)
	class USphereComponent* SphereComp;

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* MeshComp;

	// �߻�ü ������Ʈ�� �����ʹ�.
	UPROPERTY(EditDefaultsOnly)
	class UProjectileMovementComponent* MovementComp;

	//UFUNCTION()
	//void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit);
};

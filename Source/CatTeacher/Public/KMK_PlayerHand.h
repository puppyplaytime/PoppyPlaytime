// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KMK_PlayerHand.generated.h"
UCLASS()
class CATTEACHER_API AKMK_PlayerHand : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKMK_PlayerHand();
	UPROPERTY(EditAnywhere, Category = "Hand")
	class UStaticMeshComponent* hand = nullptr;
	UPROPERTY(EditAnywhere, Category = "Hand")
	TArray<class UStaticMesh*> HandMesh;
	UPROPERTY(EditAnywhere, Category = "Hand")
	class UArrowComponent* arrow;
	UPROPERTY(EditAnywhere, Category = "Hand")
	class UBoxComponent* box;
	class AKMK_Player* player;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY()
	bool isGo = false;
	bool isReverse = false;
	bool isRay = false;
	float handPos;
	FVector pos;
	FVector startPos;
	FVector endPos;
	FVector dir;
	
	UPROPERTY(EditAnywhere, Category = "Move")
	float speed = 100;

	float t = 0;
	UPROPERTY(EditAnywhere, Category = "Shoot")
	float ShootTime = 3;
	
	class UKMK_PlayerHandFSM* FSM;
	// 물건을 잡을 수 있게하는 컴포넌트 
	class UPhysicsHandleComponent* handle = nullptr;
	class UPrimitiveComponent* hitinfo;
	bool isGrab = false;

	UFUNCTION()
	// 오버랩 감지를 위한 함수 선언
	virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult);

	class AKMK_Battery* grabActor;
	float mass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AKMK_Battery> BatteryFact;
	FTransform trans;
};

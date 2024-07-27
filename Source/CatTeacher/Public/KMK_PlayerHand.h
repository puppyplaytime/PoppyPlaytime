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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY()
	bool isGo = false;
	bool isReverse = false;
	FVector handPos;
	FVector startPos;
	FVector endPos;
	FVector dir;
	UPROPERTY(EditAnywhere, Category = "Move")
	float speed = 100;
};

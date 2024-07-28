// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KMK_Battery.generated.h"

UCLASS()
class CATTEACHER_API AKMK_Battery : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKMK_Battery();
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* meshComp = nullptr;
	UPROPERTY(EditAnywhere)
	class UBoxComponent* box;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool isThrow = false;
	bool isGrab = false;
	FVector throwPos;
};

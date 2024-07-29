// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KHH_OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CATTEACHER_API UKHH_OpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UKHH_OpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere)
	FVector MoveOffset;
	FVector OriginalLocation;

	UPROPERTY(EditAnywhere)
	float MoveTime = 0.3f;

	UPROPERTY(EditAnywhere)
	float DistanceThreshold = 700.0f;

	UPROPERTY(EditAnywhere)
	bool ShouldMove = false;

	UPROPERTY(EditAnywhere)
	class APawn* Player;
	FVector CurrentLocation;
	FVector TargetLocation;
	float Speed;
	FVector NewLocation;

	void MoveDoor(float DeltaTime);	
};

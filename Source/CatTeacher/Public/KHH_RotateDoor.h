// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KHH_RotateDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CATTEACHER_API UKHH_RotateDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UKHH_RotateDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
	FRotator OpenAngle = FRotator(0, -110, 0);
	FRotator InitialRotation;

	UPROPERTY(EditAnywhere)
	float MoveTime = 1;

	UPROPERTY(EditAnywhere)
	float DistanceThreshold = 150.0f;

	UPROPERTY(EditAnywhere)
	bool ShouldMove = false;

	UPROPERTY(EditAnywhere)
	class APawn* Player;
	//class APawn* Enemy;


	FRotator CurrentRotation;
	FRotator TargetRotation;
	float Speed;
	FRotator NewRotation;

	void RotateDoor(float DeltaTime);	
		
};

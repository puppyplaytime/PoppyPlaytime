// Fill out your copyright notice in the Description page of Project Settings.


#include "LeverComponent.h"

// Sets default values for this component's properties
ULeverComponent::ULeverComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULeverComponent::BeginPlay()
{
	Super::BeginPlay();
	InitialRotation = GetOwner()->GetActorRotation();
	// ...
	
}


// Called every frame
void ULeverComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (LeverMove)
	{
		CurrentRotation = GetOwner()->GetActorRotation();
		TargetRotation = InitialRotation + OpenAngle;
		Speed = FRotator::NormalizeAxis((TargetRotation - InitialRotation).Roll) / MoveTime;
		if (Speed < 0)
		{
			Speed *= -1;
		}
		NewRotation = FMath::RInterpConstantTo(CurrentRotation, TargetRotation, DeltaTime, Speed);
		GetOwner()->SetActorRotation(NewRotation);
	}
}


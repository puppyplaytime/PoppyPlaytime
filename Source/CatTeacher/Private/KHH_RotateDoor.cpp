// Fill out your copyright notice in the Description page of Project Settings.


#include "KHH_RotateDoor.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "Engine/World.h"
#include "Components/ActorComponent.h"

// Sets default values for this component's properties
UKHH_RotateDoor::UKHH_RotateDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UKHH_RotateDoor::BeginPlay()
{
	Super::BeginPlay();
	InitialRotation = GetOwner()->GetActorRotation();
	//Enemy = (GetWorld()->GetFirstPlayerController()->GetPawn());
}


// Called every frame
void UKHH_RotateDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!Player) return;
	//if (!Player || !Enemy) return;

	// 플레이어와 문의 거리 계산
	FVector P_DistanceVector = Player->GetActorLocation() - GetOwner()->GetActorLocation();
	//FVector E_DistanceVector = Enemy->GetActorLocation() - GetOwner()->GetActorLocation();

	float Distance = P_DistanceVector.Length();

	// 문을 열어야 할 때
	if (Distance <= DistanceThreshold && !ShouldMove)
	{
		RotateDoor(DeltaTime);
	}

	//float Dis = E_DistanceVector.Length();

	//// 문을 열어야 할 때
	//if (Dis <= DistanceThreshold && !ShouldMove)
	//{
	//	RotateDoor(DeltaTime);
	//}
}

void UKHH_RotateDoor::RotateDoor(float DeltaTime)
{
	CurrentRotation = GetOwner()->GetActorRotation();
	TargetRotation = InitialRotation + OpenAngle;
	Speed = FRotator::NormalizeAxis((TargetRotation - InitialRotation).Yaw) / MoveTime;
	NewRotation = FMath::RInterpConstantTo(CurrentRotation, TargetRotation, DeltaTime, Speed);
	GetOwner()->SetActorRotation(NewRotation);
}


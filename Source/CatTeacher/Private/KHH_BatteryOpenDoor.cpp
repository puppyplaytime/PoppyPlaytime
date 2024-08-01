// Fill out your copyright notice in the Description page of Project Settings.


#include "KHH_BatteryOpenDoor.h"

// Sets default values for this component's properties
UKHH_BatteryOpenDoor::UKHH_BatteryOpenDoor()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;

    // ...
}


// Called when the game starts
void UKHH_BatteryOpenDoor::BeginPlay()
{
    Super::BeginPlay();
    OriginalLocation = GetOwner()->GetActorLocation();

}
// Called every frame
void UKHH_BatteryOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    if (ShouldMove) // 배터리가 들어갔을때 조건을 맞춰 넣어야함 // 
    {
        MoveDoor(DeltaTime);
    }
}

void UKHH_BatteryOpenDoor::MoveDoor(float DeltaTime)
{
    CurrentLocation = GetOwner()->GetActorLocation();
    TargetLocation = OriginalLocation + MoveOffset;
    Speed = FVector::Distance(OriginalLocation, TargetLocation) / MoveTime;
    NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, Speed);
    GetOwner()->SetActorLocation(NewLocation);
}
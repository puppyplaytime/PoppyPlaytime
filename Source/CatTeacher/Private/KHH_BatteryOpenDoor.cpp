// Fill out your copyright notice in the Description page of Project Settings.


#include "KHH_BatteryOpenDoor.h"
#include "KHH_Switch.h"
#include "KMK_Bat.h"

// Sets default values for this component's properties
UKHH_BatteryOpenDoor::UKHH_BatteryOpenDoor()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;

    // ...
}
// �ϰ������ : ����ġ, ���͸�, ���͸� 3���� ������ �� ������ ���Ͱ� �������Ѵ�. 
// �ؾ��ϴ��� : 
// 1. charge �� ����, IShasPush ����, 
// 1-1 ���� �Ҵ�
//  // �ʿ��� : 
// 2. ���� ���������̶��
// 3. ������ �������� ��� True�϶�
// 4. movedoor�� �Ҳ���

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
    if (GetOwner()->GetName().Contains("MG6"))
    {
        SwitchComponent = Cast<AKHH_Switch>(doors[0]);
        BatComponent = doors[1]->FindComponentByClass<UKMK_Bat>();
        BatComponent1 = doors[2]->FindComponentByClass<UKMK_Bat>();

        if (SwitchComponent->lastDoor == true && BatComponent->isHaveBat == true && BatComponent1->isHaveBat == true)
        {
            MoveDoor(DeltaTime);
        }
    }
    else {
        if (ShouldMove) // ���͸��� ������ ������ ���� �־���� // 
        {
            MoveDoor(DeltaTime);
        }
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
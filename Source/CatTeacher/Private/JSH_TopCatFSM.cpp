#include "JSH_TopCatFSM.h"
#include "JSH_Cat.h"
#include "JSH_CatDoor.h"
#include "JSH_Light.h"
#include "KHH_RotateDoor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UJSH_TopCatFSM::UJSH_TopCatFSM()
{
    PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UJSH_TopCatFSM::BeginPlay()
{
    Super::BeginPlay();

    // ���� �ʱ�ȭ
    tState = TCatState::Idle;

    // CatDoor ���� ��������
    CatDoor = Cast<AJSH_CatDoor>(UGameplayStatics::GetActorOfClass(GetWorld(), AJSH_CatDoor::StaticClass()));

    // ������ ��� AJSH_Light ���͸� �迭�� ����
    TArray<AActor*> FoundLights;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AJSH_Light::StaticClass(), FoundLights);

    for (AActor* Actor : FoundLights)
    {
        AJSH_Light* Light = Cast<AJSH_Light>(Actor);
        if (Light)
        {
            DoorLights.Add(Light);
        }
    }

    if (CatDoor)
    {
        // 'Door'��� �̸����� ������ ������ �����մϴ�.
        FName DoorName = "Door";
        CatDoor->SetActorLabel(DoorName.ToString());
    }

    if (AttackStart)
    {
        tState = TCatState::TopOpen;
    }
}

// Called every frame
void UJSH_TopCatFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    switch (tState)
    {
    case TCatState::Idle:
        IdleState(DeltaTime);
        break;

    case TCatState::Prepare:
        PrepareState(DeltaTime);
        break;
        
    case TCatState::TopOpen:
        TopOpenState(DeltaTime);
        break;
        
    case TCatState::Attack:
        AttackState();
        break;
    }

    FString myState = UEnum::GetValueAsString(tState);
    DrawDebugString(GetWorld(), GetOwner()->GetActorLocation(), myState, nullptr, FColor::Blue, 0, true, 1);
}

void UJSH_TopCatFSM::IdleState(float DeltaTime)
{
    currtime += DeltaTime;

    // ��� DoorLight�� ���ݴϴ�.
    for (AJSH_Light* Light : DoorLights)
    {
        if (Light)
        {
            Light->LightOn = false;
            Light->LightOff = true;
        }
    }

    if (currtime >= 5)
    {
        tState = TCatState::Prepare;
        currtime = 0;
    }
}

void UJSH_TopCatFSM::PrepareState(float DeltaTime)
{
    lighttime += DeltaTime;

    // ��� DoorLight�� �մϴ�.
    for (AJSH_Light* Light : DoorLights)
    {
        if (Light)
        {
            Light->LightOn = true;
            Light->LightOff = false;
        }
    }

    if (lighttime >= opentime)
    {
        tState = TCatState::TopOpen;
        DoorOpen = false;
        topStart = true;
        lighttime = 0;
    }
}

void UJSH_TopCatFSM::TopOpenState(float DeltaTime)
{
    currtime += DeltaTime;

    if (topStart)
    {
        CatDoor->fsm->isOpen = true;
        topStart = false;
    }
    
    if (CatDoor->fsm->isOpen)
    {
        if (currtime >= toptime)
        {
            tState = TCatState::Attack;
            bHasAttacked = true;
            currtime = 0;
            CatDoor->fsm->isOpen = false;
        }
    }
    else if (CatDoor->fsm->isOpen == false)
    {
        tState = TCatState::Idle;
        currtime = 0;
    }
}

void UJSH_TopCatFSM::AttackState()
{
    if (bHasAttacked)
    {
        bHasAttacked = false;
        // Attack ������ ������ �����մϴ�.
    }
}

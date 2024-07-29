#include "JSH_CatFSM.h"

#include "EngineUtils.h"
#include "JSH_Cat.h"
#include "KMK_Player.h"
#include "Kismet/GameplayStatics.h"
#include "JSH_Target.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UJSH_CatFSM::UJSH_CatFSM()
{
    PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UJSH_CatFSM::BeginPlay()
{
    Super::BeginPlay();
    
    me = Cast<AJSH_Cat>(GetOwner());

    cState = ECatState::RoundMove;

    
    // CatNab 초기 이동 속도 조절
    if (me && me->GetCharacterMovement())
    {
        me->GetCharacterMovement()->MaxWalkSpeed = 250.0f; // Set the desired max speed
    }

  
}

void UJSH_CatFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    switch (cState)
    {
    case ECatState::RoundMove:
        RoundMoveState();
        break;
        
    case ECatState::MoveWait:
        MoveWaitState();
        break;
        
    case ECatState::TrueMove:
        TrueMoveState();
        break;

    case ECatState::FalseMove:
        FalseMoveState();
        break;

    case ECatState::Ceiling:
        CeilingState();
        break;
        
    case ECatState::Discovery:
        DiscoveryState();
        break;
        
    case ECatState::Attack:
        AttackState();
        break;

    case ECatState::Blocked:
        BlockedState();
        break;
        
    case ECatState::Die:
        DieState();
        break;
    }

    FString logMsg = UEnum::GetValueAsString(cState);
    GEngine->AddOnScreenDebugMessage(0, 1, FColor::Cyan, logMsg);

    //UpdateState();
    UpdateStateFalse();
}

void UJSH_CatFSM::UpdateState()
{
    if (SelectedTag == "FCat1")
    {
        if (me && me->Tags.Contains("FCat1"))
        {
            cState = ECatState::MoveWait;
            me->Tags.Remove("FCat1");
        }
    }
    else if (SelectedTag == "FCat2")
    {
        if (me && me->Tags.Contains("FCat2"))
        {
            cState = ECatState::MoveWait;
            me->Tags.Remove("FCat2");
        }
    }
    else if (SelectedTag == "FCat3")
    {
        if (me && me->Tags.Contains("FCat3"))
        {
            cState = ECatState::MoveWait;
            me->Tags.Remove("FCat3");
        }
    }
    else if (SelectedTag == "FCat4")
    {
        if (me && me->Tags.Contains("FCat4"))
        {
            cState = ECatState::MoveWait;
            me->Tags.Remove("FCat4");
        }
    }
}

// 환상
void UJSH_CatFSM::UpdateStateFalse()
{
    if (SelectedTagFalse == "FCat1")
    {
        if (me && me->Tags.Contains("FCat1"))
        {
            cState = ECatState::MoveWait;
            me->Tags.Remove("FCat1");
        }
    }
    else if (SelectedTagFalse == "FCat2")
    {
        if (me && me->Tags.Contains("FCat2"))
        {
            cState = ECatState::MoveWait;
            me->Tags.Remove("FCat2");
        }
    }
    else if (SelectedTagFalse == "FCat3")
    {
        if (me && me->Tags.Contains("FCat3"))
        {
            cState = ECatState::MoveWait;
            me->Tags.Remove("FCat3");
        }
    }
    else if (SelectedTagFalse == "FCat4")
    {
        if (me && me->Tags.Contains("FCat4"))
        {
            cState = ECatState::MoveWait;
            me->Tags.Remove("FCat4");
        }
    }
}


void UJSH_CatFSM::RoundMoveState()
{
    static bool bMovingToTarget01_FCat1 = true; // FCat1 대상에 대한 이동 플래그
    static bool bMovingToTarget01_FCat2 = true; // FCat2 대상에 대한 이동 플래그
    static bool bMovingToTarget01_FCat3 = true; // FCat3 대상에 대한 이동 플래그
    static bool bMovingToTarget01_FCat4 = true; // FCat3 대상에 대한 이동 플래그

    TArray<AActor*> TaggedActors_FCat1;
    TArray<AActor*> TaggedActors_FCat2;
    TArray<AActor*> TaggedActors_FCat3;
    TArray<AActor*> TaggedActors_FCat4;

    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("FCat1"), TaggedActors_FCat1);
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("FCat2"), TaggedActors_FCat2);
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("FCat3"), TaggedActors_FCat3);
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("FCat4"), TaggedActors_FCat4);

    // me가 FCat1 태그를 가지고 있는지 확인
    bool bIsTagged_FCat1 = TaggedActors_FCat1.Contains(me);

    // me가 FCat2 태그를 가지고 있는지 확인
    bool bIsTagged_FCat2 = TaggedActors_FCat2.Contains(me);

    // me가 FCat3 태그를 가지고 있는지 확인
    bool bIsTagged_FCat3 = TaggedActors_FCat3.Contains(me);

    // me가 FCat4 태그를 가지고 있는지 확인
    bool bIsTagged_FCat4 = TaggedActors_FCat4.Contains(me);

    if (bIsTagged_FCat1)
    {
        // me가 FCat1 태그를 가진 경우에만 아래 코드를 실행
        FVector destination;
        if (bMovingToTarget01_FCat1)
        {
            destination = target01->GetActorLocation();
        }
        else
        {
            destination = target02->GetActorLocation();
        }

        FVector dir = destination - me->GetActorLocation();
        me->AddMovementInput(dir.GetSafeNormal());

        float distance = FVector::Dist(me->GetActorLocation(), destination);
        if (distance < ReachDistance)
        {
            // 목표 지점 도착, 이동 목표를 전환
            bMovingToTarget01_FCat1 = !bMovingToTarget01_FCat1;
        }
    }
    else if (bIsTagged_FCat2)
    {
        // me가 FCat2 태그를 가진 경우에만 아래 코드를 실행
        FVector destination;
        if (bMovingToTarget01_FCat2)
        {
            destination = target01->GetActorLocation();
        }
        else
        {
            destination = target02->GetActorLocation();
        }

        FVector dir = destination - me->GetActorLocation();
        me->AddMovementInput(dir.GetSafeNormal());

        float distance = FVector::Dist(me->GetActorLocation(), destination);
        if (distance < ReachDistance)
        {
            // 목표 지점 도착, 이동 목표를 전환
            bMovingToTarget01_FCat2 = !bMovingToTarget01_FCat2;
        }
    }
    else if (bIsTagged_FCat3)
    {
        // me가 FCat3 태그를 가진 경우에만 아래 코드를 실행
        FVector destination;
        if (bMovingToTarget01_FCat3)
        {
            destination = target01->GetActorLocation();
        }
        else
        {
            destination = target02->GetActorLocation();
        }

        FVector dir = destination - me->GetActorLocation();
        me->AddMovementInput(dir.GetSafeNormal());

        float distance = FVector::Dist(me->GetActorLocation(), destination);
        if (distance < ReachDistance)
        {
            // 목표 지점 도착, 이동 목표를 전환
            bMovingToTarget01_FCat3 = !bMovingToTarget01_FCat3;
        }
    }
    else if (bIsTagged_FCat4)
    {
        // me가 FCat3 태그를 가진 경우에만 아래 코드를 실행
        FVector destination;
        if (bMovingToTarget01_FCat4)
        {
            destination = target01->GetActorLocation();
        }
        else
        {
            destination = target02->GetActorLocation();
        }

        FVector dir = destination - me->GetActorLocation();
        me->AddMovementInput(dir.GetSafeNormal());

        float distance = FVector::Dist(me->GetActorLocation(), destination);
        if (distance < ReachDistance)
        {
            // 목표 지점 도착, 이동 목표를 전환
            bMovingToTarget01_FCat4 = !bMovingToTarget01_FCat4;
        }
    }
}

void UJSH_CatFSM::MoveWaitState()
{
    FVector destinationM = targetMiddle->GetActorLocation();
    FVector dirM = destinationM - me->GetActorLocation();
    me->AddMovementInput(dirM.GetSafeNormal());

    if (dirM.Size() < ReachDistance)
    {
        cState = ECatState::TrueMove;
    }
}

void UJSH_CatFSM::TrueMoveState()
{
    FVector destination = targetPlayer->GetActorLocation();
    FVector dir = destination - me->GetActorLocation();
    me->AddMovementInput(dir.GetSafeNormal());

    if (dir.Size() < ReachDistance)
    {
        bHasAttacked = true;
        cState = ECatState::Attack;
    }
}

void UJSH_CatFSM::FalseMoveState()
{
}

void UJSH_CatFSM::CeilingState()
{
}

void UJSH_CatFSM::DiscoveryState()
{
    if (bHasAttacked)
    {
        UE_LOG(LogTemp, Warning, TEXT("false"));
        bHasAttacked = false;
    }
}

void UJSH_CatFSM::AttackState()
{
    if (bHasAttacked)
    {
        UE_LOG(LogTemp, Warning, TEXT("Attack"));
        bHasAttacked = false;
    }
}

void UJSH_CatFSM::BlockedState()
{
}

void UJSH_CatFSM::DieState()
{
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "JSH_CatFSM.h"

#include "JSH_Cat.h"
#include "KMK_Player.h"
#include "Kismet/GameplayStatics.h"
#include "JSH_Target.h"

// Sets default values for this component's properties
UJSH_CatFSM::UJSH_CatFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UJSH_CatFSM::BeginPlay()
{
	Super::BeginPlay();
	
	// 월드에서 Player 찾아오기
	//auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), AJSH_Cat::StaticClass());
	//layer = Cast<AJSH_Cat>(actor);
	
	// 소유 객체 가져오기
	me = Cast<AJSH_Cat>(GetOwner());

	cState = ECatState::RoundMove;
	
}


// Called every frame
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

	
}


void UJSH_CatFSM::RoundMoveState()
{
    static bool bMovingToTarget01_FCat1 = true; // FCat1 대상에 대한 이동 플래그
    static bool bMovingToTarget01_FCat2 = true; // FCat2 대상에 대한 이동 플래그
    static bool bMovingToTarget01_FCat3 = true; // FCat3 대상에 대한 이동 플래그

    TArray<AActor*> TaggedActors_FCat1;
    TArray<AActor*> TaggedActors_FCat2;
    TArray<AActor*> TaggedActors_FCat3;

    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("FCat1"), TaggedActors_FCat1);
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("FCat2"), TaggedActors_FCat2);
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("FCat3"), TaggedActors_FCat3);

    // me가 FCat1 태그를 가지고 있는지 확인
    bool bIsTagged_FCat1 = TaggedActors_FCat1.Contains(me);

    // me가 FCat2 태그를 가지고 있는지 확인
    bool bIsTagged_FCat2 = TaggedActors_FCat2.Contains(me);

    // me가 FCat3 태그를 가지고 있는지 확인
    bool bIsTagged_FCat3 = TaggedActors_FCat3.Contains(me);

    if (bIsTagged_FCat1)
    {
        // me가 FCat1 태그를 가진 경우에만 아래 코드를 실행
        FVector destination = bMovingToTarget01_FCat1 ? target01->GetActorLocation() : target02->GetActorLocation();

        FVector dir = destination - me->GetActorLocation();
        me->AddMovementInput(dir.GetSafeNormal());

        if (FVector::Dist(me->GetActorLocation(), destination) < ReachDistance)
        {
            // 목표 지점 도착, 이동 목표를 전환
            bMovingToTarget01_FCat1 = !bMovingToTarget01_FCat1;
        }
    }
    else if (bIsTagged_FCat2)
    {
        // me가 FCat2 태그를 가진 경우에만 아래 코드를 실행
        FVector destination = bMovingToTarget01_FCat2 ? target01->GetActorLocation() : target02->GetActorLocation();

        FVector dir = destination - me->GetActorLocation();
        me->AddMovementInput(dir.GetSafeNormal());

        if (FVector::Dist(me->GetActorLocation(), destination) < ReachDistance)
        {
            // 목표 지점 도착, 이동 목표를 전환
            bMovingToTarget01_FCat2 = !bMovingToTarget01_FCat2;
        }
    }
    else if (bIsTagged_FCat3)
    {
        // me가 FCat3 태그를 가진 경우에만 아래 코드를 실행
        FVector destination = bMovingToTarget01_FCat3 ? target01->GetActorLocation() : target02->GetActorLocation();

        FVector dir = destination - me->GetActorLocation();
        me->AddMovementInput(dir.GetSafeNormal());

        if (FVector::Dist(me->GetActorLocation(), destination) < ReachDistance)
        {
            // 목표 지점 도착, 이동 목표를 전환
            bMovingToTarget01_FCat3 = !bMovingToTarget01_FCat3;
        }
    }

    // ---------------------------------------------
    currentTime += GetWorld()->DeltaTimeSeconds;

    if (currentTime >= stTime)
    {
        // 태그 목록
        TArray<FName> Tags = { FName("FCat1"), FName("FCat2"), FName("FCat3")};

        // 무작위로 태그 선택
        int32 RandomIndex = FMath::RandRange(0, Tags.Num() - 1);
        SelectedTag = Tags[RandomIndex]; // 선택된 태그를 저장

        // 선택된 태그를 가진 모든 액터 찾기
        TArray<AActor*> TaggedCats;
        UGameplayStatics::GetAllActorsWithTag(GetWorld(), SelectedTag, TaggedCats);
        currentTime = 0;

        // me가 선택된 태그를 가지고 있는지 확인
        bool bIsTaggedCat = TaggedCats.Contains(me);

        if (bIsTaggedCat)
        {
            UE_LOG(LogTemp, Warning, TEXT("%s is moving to Straight - State with tag %s"), *me->GetName(), *SelectedTag.ToString());
            cState = ECatState::MoveWait;
        }
    }
}

void UJSH_CatFSM::MoveWaitState()
{
    // me가 선택된 태그를 가지고 있는지 확인
    TArray<AActor*> TaggedCats;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), SelectedTag, TaggedCats);

    bool bIsTaggedCat = TaggedCats.Contains(me);

    if (bIsTaggedCat)
    {
        FVector destinationM = targetMiddle->GetActorLocation();
        FVector dirM = destinationM - me->GetActorLocation();
        me->AddMovementInput(dirM.GetSafeNormal());
        if (dirM.Size() < ReachDistance)
        {
            // 상태 전환
            cState = ECatState::TrueMove;
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("%s does not have the %s tag."), *me->GetName(), *SelectedTag.ToString());
    }
}




void UJSH_CatFSM::TrueMoveState()
{
	FVector destination = targetPlayer->GetActorLocation();
	FVector dir = destination - me->GetActorLocation();
	me->AddMovementInput(dir.GetSafeNormal());
	// 특정 위치 도달 -> AttackState()  ___ Game Over
	// (환영) 앞으로가다가 , 신호탄 맞음 -> DiscoveryState()  ___ 천천히 사라짐
	// (진짜)  앞으로가다가 player가 기믹 실행 ->  DieState() ___ destroy 후 본래 왕복 위치 spawn 
}


void UJSH_CatFSM::FalseMoveState()
{

}

void UJSH_CatFSM::CeilingState()
{
}

void UJSH_CatFSM::DiscoveryState()
{
}

void UJSH_CatFSM::AttackState()
{
}

void UJSH_CatFSM::BlockedState()
{
}

void UJSH_CatFSM::DieState()
{
}

void UJSH_CatFSM::OnDamgaeProcess()
{
}
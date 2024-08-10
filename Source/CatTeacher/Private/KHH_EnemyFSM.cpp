// Fill out your copyright notice in the Description page of Project Settings.


#include "KHH_EnemyFSM.h"
#include <AIController.h>
#include <NavigationSystem.h>
#include "KHH_Enemy.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ActorComponent.h"
#include "TimerManager.h"
#include "UObject/NoExportTypes.h"
#include "Engine/World.h"


// Sets default values for this component's properties
UKHH_EnemyFSM::UKHH_EnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UKHH_EnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	me = Cast<AKHH_Enemy>(GetOwner());
	ai = Cast<AAIController>(me->GetController());
	target = GetWorld()->GetFirstPlayerController()->GetPawn();
	//AAIController
	Speed = P_Speed;

	/*if (me)
	{
		me->OnDestroyed.AddDynamic(this, &UKHH_EnemyFSM::OnDestroyed);
	}*/
}


// Called every frame
void UKHH_EnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// ...

	FString logMsg = UEnum::GetValueAsString(mState);
	GEngine->AddOnScreenDebugMessage(1, 1, FColor::Cyan, logMsg);

	switch (mState) {
	case EEnemyState::Idle:
		IdleState();
		break;

	case EEnemyState::Move:
		MoveState();
		break;

	case EEnemyState::MoveStop:
		MoveStopState();
		break;

	case EEnemyState::Destroy:
		DestroyState();
		break;
	
	case EEnemyState::Spawn:
		SpawnState();
		break;	

	case EEnemyState::Die:
		SpawnState();
		break;
	}
}

void UKHH_EnemyFSM::IdleState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > idleDelayTime)
	{
		mState = EEnemyState::Move;
		currentTime = 0;
	}
}

void UKHH_EnemyFSM::MoveState()
{
	P_Speed = Speed;
	FVector destination = target->GetTargetLocation();
	FVector dir = destination - me->GetActorLocation();

	ai->MoveToLocation(destination);
}

void UKHH_EnemyFSM::MoveStopState()
{
	//player랑 눈이 마주치면 멈춰야함 
	//눈이 안마주치면 그대로 move, 따라가야함 
	// player의 speed를 0으로 
	P_Speed = 0;
}

void UKHH_EnemyFSM::DestroyState()
{	
	//me -> Destroy();
	//mState = EEnemyState::Spawn;
	if (me)
	{
		me->Destroy();
	}
}
void UKHH_EnemyFSM::SpawnState()
{
	// destroy되고 스폰된 상태
	// 배터리 오픈 도어에 스폰되어 있기는한데 옮길 수 있도록..
	// 특정 부분에서
	// leveMove가 true이면 
	// die state로
	mState = EEnemyState::Die;
	// 그게 아니면 딜라이트 죽음 애니메이션 
}

void UKHH_EnemyFSM::DieState()
{	
	OnMyRunStart();
	// 달려 오는 애니메이션 
}

//void UKHH_EnemyFSM::OnMyStartMove()
//{
//	
//}
//
void UKHH_EnemyFSM::OnMyRunStart()
{
	if (->LeverMove = false)
	{

	}
	// levermove 변수를 갖고 올 수 있어야함. 그래서 
}


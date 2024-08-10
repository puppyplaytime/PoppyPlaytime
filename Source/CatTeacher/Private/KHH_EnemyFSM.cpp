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
	//player�� ���� ����ġ�� ������� 
	//���� �ȸ���ġ�� �״�� move, ���󰡾��� 
	// player�� speed�� 0���� 
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
	// destroy�ǰ� ������ ����
	// ���͸� ���� ��� �����Ǿ� �ֱ���ѵ� �ű� �� �ֵ���..
	// Ư�� �κп���
	// leveMove�� true�̸� 
	// die state��
	mState = EEnemyState::Die;
	// �װ� �ƴϸ� ������Ʈ ���� �ִϸ��̼� 
}

void UKHH_EnemyFSM::DieState()
{	
	OnMyRunStart();
	// �޷� ���� �ִϸ��̼� 
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
	// levermove ������ ���� �� �� �־����. �׷��� 
}


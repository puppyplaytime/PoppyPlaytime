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
#include "LeverAnimInstance.h"
#include "LeverComponent.h"
#include "GameFramework/Actor.h"


// Sets default values for this component's properties
UKHH_EnemyFSM::UKHH_EnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//lever = Player->FindComponentByClass<ULeverComponent>();
	

	// ...s
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


void UKHH_EnemyFSM::MoveState()
{
	P_Speed = Speed;
	FVector destination = target->GetTargetLocation();
	FVector dir = destination - me->GetActorLocation();

	ai->MoveToLocation(destination);
	// destroy되고 스폰된 상태
	//배터리 오픈 도어에 스폰되어 있기는한데 옮길 수 있도록..
	// 특정 부분에서
	float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());
	if (distance < 800 && lever->LeverMove == true)
	{
		UE_LOG(LogTemp, Warning, TEXT("dfsefesdf"));
		mState = EEnemyState::Die;
		Anim->DelightState = mState;
	}
	else
	{

	}
	//else if
	//{
	//// 공격 애니메이션 
	//}
	//// leveMove가 true이면 
	//// die state로
	//mState = EEnemyState::Die;
	//// 그게 아니면 딜라이트 죽음 애니메이션 

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
	if (me)
	{
		me->Destroy();
	}
	//mState = EEnemyState::Spawn;

}
void UKHH_EnemyFSM::SpawnState()
{

	mState = EEnemyState::Move;
	Anim->DelightState = mState;
}

void UKHH_EnemyFSM::DieState()
{
//	// 달려오는 애니메이션으로 전환
}




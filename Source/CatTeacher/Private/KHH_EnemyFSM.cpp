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
	// destroy�ǰ� ������ ����
	//���͸� ���� ��� �����Ǿ� �ֱ���ѵ� �ű� �� �ֵ���..
	// Ư�� �κп���
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
	//// ���� �ִϸ��̼� 
	//}
	//// leveMove�� true�̸� 
	//// die state��
	//mState = EEnemyState::Die;
	//// �װ� �ƴϸ� ������Ʈ ���� �ִϸ��̼� 

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
//	// �޷����� �ִϸ��̼����� ��ȯ
}




// Fill out your copyright notice in the Description page of Project Settings.


#include "KHH_Teatcher_FSM.h"

// Sets default values for this component's properties
UKHH_Teatcher_FSM::UKHH_Teatcher_FSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UKHH_Teatcher_FSM::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void UKHH_Teatcher_FSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// FSM 변경 함수
	ChangeState(tState);
}
#pragma region ChangeState

void UKHH_Teatcher_FSM::ChangeState(Teacher_FSM tState)
{

	// m_state에 따른 변화
	switch (tState)
	{
	case Teacher_FSM::T_Idle:
		IdleState();
		break;
	case Teacher_FSM::T_Move:
		MoveState();
		break;
	case Teacher_FSM::T_Attack:
		AttackState();
		break;
	}
}

#pragma endregion

#pragma region IdleState
void UKHH_Teatcher_FSM::IdleState()
{

}
#pragma endregion


#pragma region MoveState
void UKHH_Teatcher_FSM::MoveState()
{

}
#pragma endregion

#pragma region AttackState
void UKHH_Teatcher_FSM::AttackState()
{

}
#pragma endregion

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KHH_Teatcher_FSM.generated.h"
// State ������
UENUM()
enum class Teacher_FSM : uint8
{
	T_Idle,
	T_Move,
	T_Attack,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CATTEACHER_API UKHH_Teatcher_FSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UKHH_Teatcher_FSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// ������Ʈ enum ������ ���� ���� ����
	Teacher_FSM tState = Teacher_FSM::T_Idle;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION()
	void ChangeState(Teacher_FSM tState);
		

	void IdleState();
	void MoveState();
	void AttackState();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "JSJ_CatNap_FSM.generated.h"
// State ������
UENUM()
enum class CATNAP_State : uint8
{
	C_Idle,
	C_Move,
	C_Attack,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CATTEACHER_API UJSJ_CatNap_FSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UJSJ_CatNap_FSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	


public:	

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	// ������Ʈ enum ������ ���� ���� ����
	UPROPERTY()
	CATNAP_State cState = CATNAP_State::C_Idle;

	void IdleState();
	void MoveState();
	void AttackState();
};

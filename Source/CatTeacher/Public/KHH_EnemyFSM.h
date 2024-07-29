// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KHH_EnemyFSM.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle,
	Move,
	Stop,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CATTEACHER_API UKHH_EnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UKHH_EnemyFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


public:
	UPROPERTY(EditAnywhere)
	bool isChange = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=FSM)
	EEnemyState mState = EEnemyState::Idle;

	void IdleState();
	void MoveState();

	UPROPERTY(EditDefaultsOnly, Category=FSM)
	float idleDelayTime = 2;
	float currentTime = 0;

	UPROPERTY(EditAnywhere, Category=FSM)
	class APawn* target;

	UPROPERTY(EditAnywhere)
	class AKHH_Enemy* me;

	UPROPERTY(EditAnywhere)
	class AAIController* ai;

	void MoveStop();
	UPROPERTY(EditDefaultsOnly, Category=FSM)
	float P_Speed;
	float Speed;

		
};

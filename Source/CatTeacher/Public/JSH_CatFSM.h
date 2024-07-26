// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "JSH_CatFSM.generated.h"


UENUM(BlueprintType)
enum class ECatState : uint8
{
	Move01,
	Move02,
	Straight,
	Attack,
	Discovery,
	Damage,
	Die,
};



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CATTEACHER_API UJSH_CatFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UJSH_CatFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	// 상태 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=FSM)
	ECatState cState = ECatState::Move01;


	

	//-----------------------------------------------------------------

	// 대기 왕복 움직임 상태
	void Move01State();

	// 대기 왕복 움직임 상태
	void Move02State();

	// 이동 상태
	void StraightState();

	// 이동 상태
	void DiscoveryState();
	
	// 공격 상태
	void AttackState();

	// 피격 상태
	void DamageState();

	// 죽음 상태
	void DieState();
	//-----------------------------------------------------------------


	

	// 왕복 타깃
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSM)
	class AJSH_Target* target01;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSM)
	class AJSH_Target* target02;


	// 소유 액터
	UPROPERTY()
	class AJSH_Cat* me;

	// 공격 범위
	UPROPERTY(EditAnywhere, Category=FSM)
	float attakcRange = 150.0f;

	// 공격 대기 시간
	UPROPERTY(EditAnywhere, Category=FSM)
	float attackDelayTime = 2.0f;

	UPROPERTY(EditAnywhere, Category=FSM)
	float rr = 150.f;

	// 피격 알림 이벤트 함수
	void OnDamgaeProcess();


};

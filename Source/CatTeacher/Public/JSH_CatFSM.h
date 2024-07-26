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
	// ���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=FSM)
	ECatState cState = ECatState::Move01;


	

	//-----------------------------------------------------------------

	// ��� �պ� ������ ����
	void Move01State();

	// ��� �պ� ������ ����
	void Move02State();

	// �̵� ����
	void StraightState();

	// �̵� ����
	void DiscoveryState();
	
	// ���� ����
	void AttackState();

	// �ǰ� ����
	void DamageState();

	// ���� ����
	void DieState();
	//-----------------------------------------------------------------


	

	// �պ� Ÿ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSM)
	class AJSH_Target* target01;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSM)
	class AJSH_Target* target02;


	// ���� ����
	UPROPERTY()
	class AJSH_Cat* me;

	// ���� ����
	UPROPERTY(EditAnywhere, Category=FSM)
	float attakcRange = 150.0f;

	// ���� ��� �ð�
	UPROPERTY(EditAnywhere, Category=FSM)
	float attackDelayTime = 2.0f;

	UPROPERTY(EditAnywhere, Category=FSM)
	float rr = 150.f;

	// �ǰ� �˸� �̺�Ʈ �Լ�
	void OnDamgaeProcess();


};

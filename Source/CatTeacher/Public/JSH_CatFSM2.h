// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "JSH_CatFSM2.generated.h"



UENUM(BlueprintType)
enum class ECatState2 : uint8
{
	RoundMove,
	MoveWait,
	TrueMove,
	FalseMove,
	Ceiling,
	Attack,
	Discovery,
	Blocked,
	Die,
};



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CATTEACHER_API UJSH_CatFSM2 : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UJSH_CatFSM2();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	// ���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=FSM)
	ECatState2 cState2 = ECatState2::RoundMove;


	

	//-----------------------------------------------------------------

	// ��� �պ� ������ ����
	void RoundMoveState();
	
	// ��¥ �̵�
	void MoveWaitState();

	// ��¥ �̵�
	void TrueMoveState();

	// ��¥ �̵�
	void FalseMoveState();

	// õ��
	void CeilingState();

	// ��¥ �߰� ����
	void DiscoveryState();
	
	// ���� ����
	void AttackState();

	// �� ��������
	void BlockedState();

	// ���� ����
	void DieState();
	//-----------------------------------------------------------------



	
	// �պ� Ÿ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSM)
	class AJSH_Target* target01;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSM)
	class AJSH_Target* target02;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSM)
	class AJSH_Target* targetMiddle;
	
	// player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSM)
	class AJSH_Target* targetPlayer;
	
	

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSM)
	//class AJSH_Cat*  AttackCat;

	
	// ���� ����
	UPROPERTY()
	class AJSH_Cat2* me;
	
	

	UPROPERTY(EditAnywhere, Category=FSM)
	float ReachDistance = 150.0f;

	// �ǰ� �˸� �̺�Ʈ �Լ�
	void OnDamgaeProcess();

	//
	UPROPERTY(EditAnywhere, Category=FSM)
	float stTime = 6;

	UPROPERTY(EditAnywhere, Category=FSM)
	float currentTime = 0;
	
	
};


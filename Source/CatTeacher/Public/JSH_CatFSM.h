// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KMK_Player.h"
#include "Components/ActorComponent.h"
#include "JSH_CatFSM.generated.h"


UENUM(BlueprintType)
enum class ECatState : uint8
{
	RoundMove,
	MoveWait,
	TrueMove,
	FalseMoveWait,
	FalseMove,
	Ceiling,
	Attack,
	Discovery,
	Blocked,
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
	ECatState cState = ECatState::RoundMove;


	

	//-----------------------------------------------------------------

	// ��� �պ� ������ ����
	void RoundMoveState();
	
	// ��¥ �̵� �غ�
	void MoveWaitState();

	// ��¥ �̵�
	void TrueMoveState();

	// ��¥ �̵� �غ�
	void FalseMoveWaitState();

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
	class AJSH_Cat* me;
	
	

	UPROPERTY(EditAnywhere, Category=FSM)
	float ReachDistance = 150.0f;
	
	UPROPERTY(EditAnywhere, Category=FSM)
	float stTime = 5;

	UPROPERTY(EditAnywhere, Category=FSM)
	float currentTime = 0;

public:
	FTimerHandle TagSelectionTimerHandle;

	//------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FSM)
	FName SelectedTag;

	// ���� ���� �Լ�
	void UpdateState();
	//------------


	//------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FSM)
	FName SelectedTagFalse;
	
	void UpdateStateFalse();
	//------------

	
	bool bHasAttacked = false;

	
};





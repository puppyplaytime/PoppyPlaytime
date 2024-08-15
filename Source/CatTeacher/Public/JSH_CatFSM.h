// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KMK_Player.h"
#include "Components/ActorComponent.h"
#include "JSH_CatFSM.generated.h"


UENUM(BlueprintType)
enum class ECatState : uint8
{
	Idle,
	RoundMove,
	MoveWait,
	TrueMove,
	FalseMoveWait,
	FalseMove,
	Ceiling,
	Attack,
	Discovery,
	top,
	Die,
	ending,
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

	// ���ͷ��� �� �̵� ��� ����
	void IdleState(float DeltaTime);
	
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
	void TopState();

	// ���� ����
	void DieState();

	void EndingState();
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

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCameraShakeBase> ccamera;
	
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

	//bool FalseDie = false;

	
	// idle ���� ���� 
	bool IdleTeleport = false;
	
	UPROPERTY(EditAnywhere, Category = "Control")
	float idletime = 15;
	
	float idlecurrtime = 0;


	// Switch ���� -> idle ���� ����
	bool SwSt1 = false;
	bool SwSt2 = false;
	bool SwSt3 = false;
	bool SwSt4 = false;

	// �̵� �ӵ� ����
	UPROPERTY(EditAnywhere, Category = "Control")
	float RoundMoveSpeed = 200.0f;

	UPROPERTY(EditAnywhere, Category = "Control")
	float AttackMoveSpeed = 50.0f;


	// 03, 04 ���͸� ����
	UPROPERTY()
	class UKMK_Bat* BatComponent;

	// ó�� 03, 04 ���͸� �ϋ� , 03, 04 ����� ���� ����
	bool FirstBatTrue03 = true;
	bool FirstBatTrue04 = true;


	//
	void BatCatStop();

	void batoff();

	FVector tt;

	// õ�� ��
	UPROPERTY()
	class AJSH_CatDoor* CatDoor;
	
	bool DoorOpen = false;
	bool topStart = false;
	
	bool ending = false;


	void End();


	// �����Ҷ� �ٸ� ���� ����
	// bool AttackCatDestroy = true;

	UPROPERTY()
	class AJSH_Ending* EndingHelper;
};





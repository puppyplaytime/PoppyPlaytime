// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KMK_PlayerHandFSM.generated.h"

// �÷��̾� �尩�� ���� FSM
UENUM()
enum class PlayerHandFSM
{
	Normal,
	JumpPack,
	GunPack,
	Energy
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CATTEACHER_API UKMK_PlayerHandFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UKMK_PlayerHandFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	// �÷��̾� ��ü ������ ����
	class AKMK_Player* Player = nullptr;
	
	class UCharacterMovementComponent* movementComp = nullptr;
	UPROPERTY(EditAnywhere)
	PlayerHandFSM PState = PlayerHandFSM::Normal;

	UPROPERTY()
	float JumpPower = -1;
	// ������ ���� Ȯ�ο� ����
	UPROPERTY(EditAnywhere, Category = "Charge")
	bool isCharge = false;
	float t = 0;
	UPROPERTY(EditAnywhere, Category = "Charge")
	float chargeTime = 10;
	// fsm ����� ȣ��Ǵ� �Լ���
	void NormalHand();
	void JumpHand();
	void GunHand();
	void EnergyHand();



		
};

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
	UPROPERTY()
	class AKMK_Player* Player = nullptr;
	UPROPERTY()
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

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AKMK_Bullet> bulletFact;
	UPROPERTY(EditDefaultsOnly)
	FTransform bulletTrans;

	// fsm ����� ȣ��Ǵ� �Լ���
	void NormalHand();
	void JumpHand();
	void GunHand();
	void EnergyHand();

	bool isJump = false;
	bool isFire = false;

	int cnt = 0;
// ���̺� ��Ƽ����
	UPROPERTY(EditDefaultsOnly)
	TArray<class UMaterialInterface*> cableMat;
	
	UPROPERTY(EditAnywhere)
	TArray<class UMaterial*> matArrays;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "KMK_Player.generated.h"

UCLASS()
class CATTEACHER_API AKMK_Player : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AKMK_Player();
#pragma region CreateVar

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class USpringArmComponent* springArm = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	class UCameraComponent* camera = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Component")
	class UKMK_PlayerHandFSM* FSM = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Modeling")
	class USpringArmComponent* GrabSpringArm = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Modeling")
	class UStaticMeshComponent* armMesh = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Ray")
	class UKMK_PlayerRay* playerRay = nullptr;

	UPROPERTY(EditAnywhere, Category = "Hand")
	TArray<class UCableComponent*> CableComp;
	UPROPERTY(EditAnywhere, Category = "Hand")
	TArray<class USceneComponent*> SceneComp;

	// Player에게 pawn sensor 만들기
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPawnSensingComponent* sensor;

#pragma endregion

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY()
	class UCharacterMovementComponent* movementComp = nullptr;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
#pragma region HandActorFact
	UPROPERTY(EditAnywhere, Category = "Hand")
	TSubclassOf<class AKMK_PlayerHand> LHandFact;
	UPROPERTY(EditAnywhere, Category = "Hand")
	TSubclassOf<class AKMK_PlayerHand> RHandFact;
	UPROPERTY(EditAnywhere, Category = "Hand")
	TSubclassOf<class AKMK_Battery> batteryFact;

	class AKMK_PlayerHand* LHand;
	class AKMK_PlayerHand* RHand;

	class AKMK_Battery* LBat;
	class AKMK_Battery* RBat;

	class UStaticMeshComponent* LMeshComp;
	class UStaticMeshComponent* RMeshComp;
#pragma endregion

#pragma region InputVaribles
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* IMC_Mapping = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Move = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Turn = nullptr;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputAction* IA_Jump = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputAction* IA_Sit = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputAction* IA_Run = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputAction* IA_ClickE = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputAction* IA_Click1 = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputAction* IA_Click2 = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputAction* IA_Click3 = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputAction* IA_ClickR = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputAction* IA_ClickL = nullptr;
#pragma endregion
#pragma region variables
	FVector dir = FVector(0);

	UPROPERTY(EditAnywhere, Category = "Variables")
	float speed = 600.f;
	UPROPERTY()
	float jumpPower = 0;
	UPROPERTY()
	bool isRight = false;
	UPROPERTY()
	bool isLeft = false;

	UPROPERTY()
	bool isIntarctive = false;
#pragma endregion
#pragma region Function
	// ȸ��ó��
	void InputTurn(const struct FInputActionValue& value);
	// �¿� �̵�
	void InputMove(const struct FInputActionValue& value);
	// ����
	void InputJump(const struct FInputActionValue& value);
	// �ɱ�
	void InputSit(const struct FInputActionValue& value);
	void InputStand(const struct FInputActionValue& value);
	// �޸���
	void InputRun(const struct FInputActionValue& value);
	void InputWalk(const struct FInputActionValue& value);
	// ��ȣ�ۿ�
	void InputE(const struct FInputActionValue& value);
	// ���� �ٲٱ�
	void InputNum1(const struct FInputActionValue& value);
	void InputNum2(const struct FInputActionValue& value);
	void InputNum3(const struct FInputActionValue& value);

	// ���콺 Ŭ�� 
	// 1. ongoing�϶�, �����Ÿ� ���� ��ü�� ������ ��ü�� ��� ���·� ����
	// 2. cancle �Ǹ�, ray �������� �����
	// 3. ���� �����϶�, ĵ���Ǹ� ray���
	// ������
	void InputMR(const struct FInputActionValue& value);
	void InputMRComp(const struct FInputActionValue& value);
	// �޼�
	void InputML(const struct FInputActionValue& value);
	void InputMLComp(const struct FInputActionValue& value);

#pragma endregion
#pragma region ray variables
	FVector startPos;
	FVector endPos;
	FVector endPos1;
	UPROPERTY(EditAnywhere)
	float rayDis = 1000;
	UPROPERTY(EditAnywhere)
	float rayDis1 = 2000;
#pragma endregion
#pragma region Overlap
	UFUNCTION()
	void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit);
	// 오버랩 감지를 위한 함수 선언
	/*virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);*/
#pragma endregion
};

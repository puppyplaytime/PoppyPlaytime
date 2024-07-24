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
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class USpringArmComponent* springArm;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class UCameraComponent* camera;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY()
	class UCharacterMovementComponent* movementComp;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
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

	bool isRight = false;
	bool isLeft = false;
#pragma endregion


#pragma region Function
	// 회전처리
	void InputTurn(const struct FInputActionValue& value);
	// 좌우 이동
	void InputMove(const struct FInputActionValue& value);
	// 점프
	void InputJump(const struct FInputActionValue& value);
	// 앉기
	void InputSit(const struct FInputActionValue& value);
	void InputStand(const struct FInputActionValue& value);
	// 달리기
	void InputRun(const struct FInputActionValue& value);
	void InputWalk(const struct FInputActionValue& value);
	// 상호작용
	void InputE(const struct FInputActionValue& value);
	// 무기 바꾸기
	void InputNum1(const struct FInputActionValue& value);
	void InputNum2(const struct FInputActionValue& value);
	void InputNum3(const struct FInputActionValue& value);

	// 마우스 클릭 
	// 1. ongoing일때, 일정거리 내에 물체가 있으면 물체를 잡기 상태로 변경
	// 2. cancle 되면, ray 못나가게 만들기
	// 3. 잡은 상태일때, 캔슬되면 ray취소
	// 오른손
	void InputMR(const struct FInputActionValue& value);
	void InputMRComp(const struct FInputActionValue& value);
	// 왼손
	void InputML(const struct FInputActionValue& value);
	void InputMLComp(const struct FInputActionValue& value);
#pragma endregion




};

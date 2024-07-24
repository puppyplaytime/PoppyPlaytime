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
	class UInputMappingContext* IMC_Mapping;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Move;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Turn;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputAction* IA_Jump;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputAction* IA_Sit;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputAction* IA_Run;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputAction* IA_ClickE;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputAction* IA_Click1;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputAction* IA_Click2;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputAction* IA_Click3;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputAction* IA_ClickR;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputAction* IA_ClickL;
#pragma endregion
#pragma region variables
	FVector dir;
	UPROPERTY(EditAnywhere)
	float speed = 600.f;

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
	// 오른손
	void InputMR(const struct FInputActionValue& value);
	void InputMRComp(const struct FInputActionValue& value);
	// 왼손
	void InputML(const struct FInputActionValue& value);
	void InputMLComp(const struct FInputActionValue& value);
#pragma endregion




};

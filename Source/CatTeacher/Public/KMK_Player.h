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
	// ������
	void InputMR(const struct FInputActionValue& value);
	void InputMRComp(const struct FInputActionValue& value);
	// �޼�
	void InputML(const struct FInputActionValue& value);
	void InputMLComp(const struct FInputActionValue& value);
#pragma endregion




};

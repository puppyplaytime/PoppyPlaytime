// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK_Player.h"
#include "Engine/SkeletalMesh.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AKMK_Player::AKMK_Player()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// 1. 1인칭 카메라 만들기
	// 1-1 spring arm 붙이기
	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArm->SetupAttachment(GetMesh());
	springArm->SetRelativeLocation(FVector(0, 0, 50));
	springArm->TargetArmLength = -50.f;
	springArm->bUsePawnControlRotation = true;
	// 1-2 camera 붙이기
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(springArm);
	camera->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = true;

	JumpMaxCount = 1;
	
}

// Called when the game starts or when spawned
void AKMK_Player::BeginPlay()
{
	Super::BeginPlay();
	movementComp = GetCharacterMovement();
	jumpPower = movementComp->JumpZVelocity;
	auto* pc = Cast<APlayerController>(Controller);
	if (pc)
	{
		UEnhancedInputLocalPlayerSubsystem* subSys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		if (subSys)
		{
			subSys->AddMappingContext(IMC_Mapping, 0);
		}
	}
}

// Called every frame
void AKMK_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// 이동하기
	// 회전방향으로 이동
	// 1. controller rotation을 통해 transform 생성
	FTransform t = FTransform(GetControlRotation());
	dir = t.TransformVector(dir);
	// 이동 인풋
	AddMovementInput(dir, speed * DeltaTime);
	dir = FVector::ZeroVector;
}

// Called to bind functionality to input
void AKMK_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// 플레이어 인풋 처리
	auto playerInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	if (playerInput)
	{
		// 키 바인딩
		// 회전 바인딩
		playerInput->BindAction(IA_Turn, ETriggerEvent::Triggered, this, &AKMK_Player::InputTurn);
		// 움직임 바인딩
		playerInput->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AKMK_Player::InputMove);
		// 점프 바인딩
		playerInput->BindAction(IA_Jump, ETriggerEvent::Started, this, &AKMK_Player::InputJump);
		// 앉기 바인딩
		playerInput->BindAction(IA_Sit, ETriggerEvent::Started, this, &AKMK_Player::InputSit);
		playerInput->BindAction(IA_Sit, ETriggerEvent::Completed, this, &AKMK_Player::InputStand);
		// 달리기 바인딩
		playerInput->BindAction(IA_Run, ETriggerEvent::Started, this, &AKMK_Player::InputRun);
		playerInput->BindAction(IA_Run, ETriggerEvent::Completed, this, &AKMK_Player::InputWalk);
		// 상호작용
		playerInput->BindAction(IA_ClickE, ETriggerEvent::Started, this, &AKMK_Player::InputE);
		// 손 변경 바인딩
		playerInput->BindAction(IA_Click1, ETriggerEvent::Started, this, &AKMK_Player::InputNum1);
		playerInput->BindAction(IA_Click2, ETriggerEvent::Started, this, &AKMK_Player::InputNum2);
		playerInput->BindAction(IA_Click3, ETriggerEvent::Started, this, &AKMK_Player::InputNum3);
		// 마우스 클릭
		// 오른손
		playerInput->BindAction(IA_ClickR, ETriggerEvent::Ongoing, this, &AKMK_Player::InputMR);
		playerInput->BindAction(IA_ClickR, ETriggerEvent::Canceled, this, &AKMK_Player::InputMRComp);
		// 왼손
		playerInput->BindAction(IA_ClickL, ETriggerEvent::Ongoing, this, &AKMK_Player::InputML);
		playerInput->BindAction(IA_ClickL, ETriggerEvent::Canceled, this, &AKMK_Player::InputMLComp);
	}

}

#pragma region MouseRotation
// 회전처리 함수
void AKMK_Player::InputTurn(const struct FInputActionValue& value)
{
	FVector2D v = value.Get<FVector2D>();
	AddControllerPitchInput(-v.Y);
	AddControllerYawInput(v.X);
}


#pragma endregion
#pragma region Move
// 이동관련
void AKMK_Player::InputMove(const struct FInputActionValue& value)
{
	FVector2D v = value.Get<FVector2D>();
	dir.X = v.X;
	dir.Y = v.Y;

	dir.Normalize();
}
#pragma endregion
#pragma region Jump
// 점프
void AKMK_Player::InputJump(const struct FInputActionValue& value)
{
	Jump();
}
#pragma endregion
#pragma region Sit
// 앉기
void AKMK_Player::InputSit(const struct FInputActionValue& value)
{
	springArm->SetRelativeLocation(FVector(0, 0, 0));
}
// 일어나기
void AKMK_Player::InputStand(const struct FInputActionValue& value)
{
	springArm->SetRelativeLocation(FVector(0, 0, 50));
}

#pragma endregion
#pragma region Run
// 달리기
void AKMK_Player::InputRun(const struct FInputActionValue& value)
{
	movementComp->MaxWalkSpeed = speed * 2;
}
// 걷기
void AKMK_Player::InputWalk(const struct FInputActionValue& value)
{
	movementComp->MaxWalkSpeed = speed;
}

#pragma endregion
#pragma region interaction
// 상호작용
void AKMK_Player::InputE(const struct FInputActionValue& value)
{
	GEngine->AddOnScreenDebugMessage(0, 1, FColor::White, FString::Printf(TEXT("E클릭")));
}
#pragma endregion
#pragma region ChangeHand
// 기본 손
void AKMK_Player::InputNum1(const struct FInputActionValue& value)
{
	GEngine->AddOnScreenDebugMessage(0, 1, FColor::White, FString::Printf(TEXT("1번")));
}

// 에너지 충전 손
void AKMK_Player::InputNum2(const struct FInputActionValue& value)
{
	GEngine->AddOnScreenDebugMessage(0, 1, FColor::White, FString::Printf(TEXT("2번")));
}
// 점프 손
void AKMK_Player::InputNum3(const struct FInputActionValue& value)
{
	GEngine->AddOnScreenDebugMessage(0, 1, FColor::White, FString::Printf(TEXT("3번")));
}

#pragma endregion
#pragma region MouseRight click
// 오른쪽 클릭시
void AKMK_Player::InputMR(const struct FInputActionValue& value)
{
	GEngine->AddOnScreenDebugMessage(0, 1, FColor::White, FString::Printf(TEXT("Right")));
	isRight = true;
}
void AKMK_Player::InputMRComp(const struct FInputActionValue& value)
{
	GEngine->AddOnScreenDebugMessage(0, 1, FColor::White, FString::Printf(TEXT("Right cancel")));
	isRight = false;
}

#pragma endregion
#pragma region MouseLeft Click
// 왼쪽 클릭시
void AKMK_Player::InputML(const struct FInputActionValue& value)
{
	GEngine->AddOnScreenDebugMessage(0, 1, FColor::White, FString::Printf(TEXT("Left")));
	isLeft = true;
}
void AKMK_Player::InputMLComp(const struct FInputActionValue& value)
{
	GEngine->AddOnScreenDebugMessage(0, 1, FColor::White, FString::Printf(TEXT("Left cancel")));
	// isLeft = false;
}

#pragma endregion

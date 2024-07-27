// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK_Player.h"

#include "InstancedReferenceSubobjectHelper.h"
#include "KMK_PlayerHandFSM.h"
#include "Engine/SkeletalMesh.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "Engine/GameEngine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "KMK_PlayerRay.h"
#include "Components/ArrowComponent.h"
#include "KMK_PlayerHand.h"
#include "../../../../Plugins/Runtime/CableComponent/Source/CableComponent/Classes/CableComponent.h"

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
	// 1-3 모델링 설정
#pragma region Modeling
	// 팔
	GrabSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("GrabSpringArm"));
	GrabSpringArm->SetupAttachment(GetMesh());
	GrabSpringArm->SetRelativeLocation(FVector(0, 0, 50));
	GrabSpringArm->TargetArmLength = -50.f;
	GrabSpringArm->bUsePawnControlRotation = true;
	// 팔 매쉬 붙이기
	armMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GrabpackMesh"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/Project/Modeling/Player/source/pibot_MainMesh.pibot_MainMesh'"));
	if (tempMesh.Succeeded())
	{
		armMesh->SetStaticMesh(tempMesh.Object);
	}
	armMesh->SetupAttachment(GrabSpringArm);
	armMesh->SetRelativeLocationAndRotation(FVector(0, 0, 0), FRotator(0, -90, 0));
	armMesh->SetRelativeScale3D(FVector(1.5f));

	Rcable = CreateDefaultSubobject<UCableComponent>(TEXT("RCable"));
	Rcable->CableWidth = 3;
	Rcable->SetupAttachment(GrabSpringArm);
	Rcable->SetRelativeLocation(FVector(30, 20, -13));

	Lcable = CreateDefaultSubobject<UCableComponent>(TEXT("LCable"));
	Lcable->CableWidth = 3;
	Lcable->SetupAttachment(GrabSpringArm);
	Lcable->SetRelativeLocation(FVector(30, -20, -13));
#pragma endregion

	// 점프 횟수 제한
	JumpMaxCount = 1;
	// FSM 붙이기
	FSM = CreateDefaultSubobject<UKMK_PlayerHandFSM>(TEXT("FSM"));
	playerRay = CreateDefaultSubobject<	UKMK_PlayerRay>(TEXT("RAY"));
	
}

// Called when the game starts or when spawned
void AKMK_Player::BeginPlay()
{
	Super::BeginPlay();
#pragma region Create Hand
	auto* rActor = GetWorld()->SpawnActor(RHandFact);
	rActor->AttachToComponent(GrabSpringArm, FAttachmentTransformRules::KeepRelativeTransform);
	rActor->SetActorRelativeLocation(FVector(30, 20, -13));
	rActor->SetActorRotation(FRotator(0, -90, 0));
	rActor->SetActorRelativeScale3D(FVector(1.5f));
	auto* lActor = GetWorld()->SpawnActor(LHandFact);
	lActor->AttachToComponent(GrabSpringArm, FAttachmentTransformRules::KeepRelativeTransform);
	lActor->SetActorRelativeLocation(FVector(30, -20, -13));
	lActor->SetActorRotation(FRotator(0, -90, 0));
	lActor->SetActorRelativeScale3D(FVector(1.5f));

	RHand = Cast<AKMK_PlayerHand>(rActor);
	Rcable->SetAttachEndTo(rActor, rActor->GetFName());
	Rcable->EndLocation = FVector(0);
	LHand = Cast<AKMK_PlayerHand>(lActor);
	Lcable->SetAttachEndTo(rActor, lActor->GetFName());
	Lcable->EndLocation = FVector(0);
	RMeshComp = RHand->hand;
	//LMeshComp = LHand->hand;
#pragma endregion

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

	// 계속 레이 쏘기
	startPos = camera->GetComponentLocation();
	endPos = startPos + camera->GetForwardVector() * rayDis;
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
		playerInput->BindAction(IA_ClickR, ETriggerEvent::Started, this, &AKMK_Player::InputMR);
		// playerInput->BindAction(IA_ClickR, ETriggerEvent::Canceled, this, &AKMK_Player::InputMRComp);
		playerInput->BindAction(IA_ClickR, ETriggerEvent::Completed, this, &AKMK_Player::InputMRComp);
		// 왼손
		playerInput->BindAction(IA_ClickL, ETriggerEvent::Started, this, &AKMK_Player::InputML);
		// playerInput->BindAction(IA_ClickL, ETriggerEvent::Canceled, this, &AKMK_Player::InputMLComp);
		playerInput->BindAction(IA_ClickL, ETriggerEvent::Completed, this, &AKMK_Player::InputMLComp);
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
	movementComp->JumpZVelocity = jumpPower;
	Jump();
}
#pragma endregion
#pragma region Sit
// 앉기
void AKMK_Player::InputSit(const struct FInputActionValue& value)
{
	springArm->SetRelativeLocation(FVector(0, 0, 0));
	GrabSpringArm->SetRelativeLocation(FVector(0, 0, 0));
}
// 일어나기
void AKMK_Player::InputStand(const struct FInputActionValue& value)
{
	springArm->SetRelativeLocation(FVector(0, 0, 50));
	GrabSpringArm->SetRelativeLocation(FVector(0, 0, 50));
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
	isIntarctive = true;
	GEngine->AddOnScreenDebugMessage(0, 1, FColor::White, FString::Printf(TEXT("E클릭")));
}
#pragma endregion
#pragma region ChangeHand
// 기본 손
void AKMK_Player::InputNum1(const struct FInputActionValue& value)
{
	RMeshComp->SetStaticMesh(RHand->HandMesh[0]);
	FSM->PState = PlayerHandFSM::Normal;
}

// 에너지 충전 손
void AKMK_Player::InputNum2(const struct FInputActionValue& value)
{
	RMeshComp->SetStaticMesh(RHand->HandMesh[1]);
	FSM->PState = PlayerHandFSM::GunPack;
}
// 점프 손
void AKMK_Player::InputNum3(const struct FInputActionValue& value)
{
	RMeshComp->SetStaticMesh(RHand->HandMesh[2]);
	FSM->PState = PlayerHandFSM::JumpPack;
}

#pragma endregion
#pragma region MouseRight click
// 오른쪽 클릭시
void AKMK_Player::InputMR(const struct FInputActionValue& value)
{
	GEngine->AddOnScreenDebugMessage(0, 1, FColor::White, FString::Printf(TEXT("Right")));
	playerRay->SetRayPos(RHand->arrow->GetComponentLocation(), RHand->arrow->GetComponentLocation() + camera->GetForwardVector() * rayDis);
	isRight = true;
}
void AKMK_Player::InputMRComp(const struct FInputActionValue& value)
{
	GEngine->AddOnScreenDebugMessage(0, 1, FColor::White, FString::Printf(TEXT("Right cancel")));
	playerRay->isRay = false;
	isRight = false;
}

#pragma endregion
#pragma region MouseLeft Click
// 왼쪽 클릭시
void AKMK_Player::InputML(const struct FInputActionValue& value)
{
	GEngine->AddOnScreenDebugMessage(0, 1, FColor::White, FString::Printf(TEXT("Left")));
	playerRay->SetRayPos(LHand->arrow->GetComponentLocation(), endPos);
	isLeft = true;
}
void AKMK_Player::InputMLComp(const struct FInputActionValue& value)
{
	GEngine->AddOnScreenDebugMessage(0, 1, FColor::White, FString::Printf(TEXT("Left cancel")));
	playerRay->isRay = false;
	isLeft = false;
}
#pragma endregion

#pragma region overlapFunction
void AKMK_Player::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	if (Other->GetActorLabel().Contains("Floor"))
	{
		FSM->PState = PlayerHandFSM::Normal;
	}
}
#pragma endregion



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
#include "Perception/PawnSensingComponent.h"
#include "KMK_Battery.h"
#include "Components/BoxComponent.h"
#include "KHH_Enemy.h"

// Sets default values
AKMK_Player::AKMK_Player()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// 1. 1인칭 카메라 만들기
	// 1-1 spring arm 붙이기
	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArm->SetupAttachment(GetMesh(), "head");
	springArm->SetRelativeLocation(FVector(-20, 0, 0));
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
	GrabSpringArm->SetRelativeLocation(FVector(0, 0, 140));
	GrabSpringArm->TargetArmLength = -50.f;
	GrabSpringArm->bUsePawnControlRotation = true;
	// 팔이 카메라보다 늦게 쫓아오게 만들기 위함
	GrabSpringArm->bEnableCameraLag = true;
	GrabSpringArm->bEnableCameraRotationLag = true;
	GrabSpringArm->CameraLagSpeed = 15.f;
	GrabSpringArm->CameraRotationLagSpeed = 15.f;
	GrabSpringArm->CameraLagMaxDistance = 3;

	// 팔 매쉬 붙이기
	armMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GrabpackMesh"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/Project/Modeling/Player/source/pibot_MainMesh.pibot_MainMesh'"));
	if (tempMesh.Succeeded())
	{
		armMesh->SetStaticMesh(tempMesh.Object);
	}
	armMesh->SetupAttachment(GrabSpringArm);
	armMesh->SetRelativeRotation(FRotator(0, -90, 0));
	armMesh->SetRelativeScale3D(FVector(2.f));

	CableComp.Add(CreateDefaultSubobject<UCableComponent>(TEXT("RCable")));
	SceneComp.Add(CreateDefaultSubobject<USceneComponent>(TEXT("RScene")));
	CableComp[0]->SetupAttachment(GrabSpringArm);
	SceneComp[0]->SetupAttachment(GrabSpringArm);
	CableComp[0]->SetRelativeLocationAndRotation(FVector(40, 27, -16), FRotator(0, 0, 0));
	SceneComp[0]->SetRelativeLocationAndRotation(FVector(40, 27, -16), FRotator(0, 0, 0));
	CableComp[0]->SetCollisionProfileName("Hand");
	CableComp[0]->CableWidth = 3;
	CableComp[0]->EndLocation = FVector(0);
	
	// SceneComp.Add(CreateDefaultSubobject<USceneComponent>(TEXT("LScene")));
	CableComp.Add(CreateDefaultSubobject<UCableComponent>(TEXT("LCable")));
	SceneComp.Add(CreateDefaultSubobject<USceneComponent>(TEXT("LScene")));
	CableComp[1]->SetupAttachment(GrabSpringArm);
	SceneComp[1]->SetupAttachment(GrabSpringArm);
	CableComp[1]->SetRelativeLocationAndRotation(FVector(40, -27, -16), FRotator(0, 0, 0));
	SceneComp[1]->SetRelativeLocationAndRotation(FVector(40, -27, -16), FRotator(0, 0, 0));
	CableComp[1]->SetCollisionProfileName("Hand");
	CableComp[1]->CableWidth = 3;
	CableComp[1]->EndLocation = FVector(0);

#pragma endregion

	// 점프 횟수 제한
	JumpMaxCount = 1;
	// FSM 붙이기
	FSM = CreateDefaultSubobject<UKMK_PlayerHandFSM>(TEXT("FSM"));
	playerRay = CreateDefaultSubobject<	UKMK_PlayerRay>(TEXT("RAY"));
	
	// 센서 만들기
	sensor = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Sensor"));
	sensor->bOnlySensePlayers = false;

}

// Called when the game starts or when spawned
void AKMK_Player::BeginPlay()
{
	Super::BeginPlay();
#pragma region Create Hand
	FTransform t = SceneComp[0]->GetRelativeTransform();
	RHand = GetWorld()->SpawnActor<AKMK_PlayerHand>(RHandFact, t);
	RBat = GetWorld()->SpawnActor<AKMK_Battery>(batteryFact, t);
	RBat->meshComp->SetSimulatePhysics(false);
	RBat->meshComp->SetEnableGravity(false);
	RHand->AttachToComponent(GrabSpringArm, FAttachmentTransformRules::KeepRelativeTransform);
	RBat->AttachToComponent(GrabSpringArm, FAttachmentTransformRules::KeepRelativeTransform);
	RHand->SetActorRelativeLocation(FVector(40, 27, -16));
	RBat->SetActorRelativeLocation(FVector(55, 26, 8));
	RBat->SetActorScale3D(FVector(0.55f));
	RHand->SetActorRotation(FRotator(0, -90, 0));
	RBat->meshComp->SetVisibility(false);
	RBat->meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	

	FTransform t1 = SceneComp[1]->GetRelativeTransform();
	LHand = GetWorld()->SpawnActor<AKMK_PlayerHand>(LHandFact, t1);
	LBat = GetWorld()->SpawnActor<AKMK_Battery>(batteryFact, t1);
	LBat->meshComp->SetSimulatePhysics(false);
	LBat->meshComp->SetEnableGravity(false);
	LHand->AttachToComponent(GrabSpringArm, FAttachmentTransformRules::KeepRelativeTransform);
	LBat->AttachToComponent(GrabSpringArm, FAttachmentTransformRules::KeepRelativeTransform);
	LHand->SetActorRelativeLocation(FVector(40, -27, -16));
	LHand->SetActorRotation(FRotator(0, -90, 0));
	LBat->SetActorRelativeLocation(FVector(55, -26, 8));
	LBat->SetActorScale3D(FVector(0.55f));
	LHand->SetActorRelativeScale3D(FVector(2.f));
	LBat->meshComp->SetVisibility(false);
	LBat->meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	RMeshComp = RHand->hand;
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
	movementComp->GetNavAgentPropertiesRef().bCanCrouch = true;
	LHand->FSM = FSM;
	RHand->FSM = FSM;

	// sensor->OnSeePawn.AddDynamic(this, &AKMK_Player::BroadcastOnSeePawn);
}

// Called every frame
void AKMK_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RBat->hand = RHand;
	LBat->hand = LHand;
	// 이동하기
	// 회전방향으로 이동
	// 1. controller rotation을 통해 transform 생성
	RHand->startPos = SceneComp[0]->GetComponentLocation();
	LHand->startPos = SceneComp[1]->GetComponentLocation();
	FSM->bulletTrans = RHand->arrow->GetComponentTransform();
	FTransform t = FTransform(GetControlRotation());
	dir = t.TransformVector(dir);
	// 이동 인풋
	AddMovementInput(dir, speed * DeltaTime);
	dir = FVector::ZeroVector;

	// 계속 레이 쏘기
	startPos = camera->GetComponentLocation();
	endPos = startPos + camera->GetForwardVector() * rayDis;
	endPos1 = startPos + camera->GetForwardVector() * rayDis1;
	CableComp[0]->SetAttachEndTo(RHand, NAME_None);
	CableComp[1]->SetAttachEndTo(LHand, NAME_None);
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
	Crouch();
	springArm->SetRelativeLocation(FVector(-70, 10, 0));
	GrabSpringArm->SetRelativeLocation(FVector(0, 0, 90));
}
// 일어나기
void AKMK_Player::InputStand(const struct FInputActionValue& value)
{
	UnCrouch();
	springArm->SetRelativeLocation(FVector(-20, 0, 0));
	GrabSpringArm->SetRelativeLocation(FVector(0, 0, 140));
}

#pragma endregion
#pragma region Run
// 달리기
void AKMK_Player::InputRun(const struct FInputActionValue& value)
{
	movementComp->MaxWalkSpeed = speed * 2;
	springArm->SetRelativeLocation(FVector(-10, -5, 0));
}
// 걷기
void AKMK_Player::InputWalk(const struct FInputActionValue& value)
{
	movementComp->MaxWalkSpeed = speed;
	springArm->SetRelativeLocation(FVector(-20, 0, 0));
}

#pragma endregion
#pragma region interaction
// 상호작용
void AKMK_Player::InputE(const struct FInputActionValue& value)
{
	isIntarctive = true;
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
	playerRay->SetRayPos(startPos, endPos);
	RHand->box->SetCollisionProfileName("Hand");
	isRight = true;
}
void AKMK_Player::InputMRComp(const struct FInputActionValue& value)
{
	playerRay->isRay = false;
	isRight = false;
}

#pragma endregion
#pragma region MouseLeft Click
// 왼쪽 클릭시
void AKMK_Player::InputML(const struct FInputActionValue& value)
{
	playerRay->SetRayPos(startPos, endPos);
	LHand->box->SetCollisionProfileName("Hand");
	isLeft = true;
}
void AKMK_Player::InputMLComp(const struct FInputActionValue& value)
{
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

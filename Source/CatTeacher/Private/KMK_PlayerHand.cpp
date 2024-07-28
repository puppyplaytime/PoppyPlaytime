// Fill out your copyright notice in the Description page of Project Settings.


#include "../../../../Plugins/Runtime/CableComponent/Source/CableComponent/Classes/CableComponent.h"
#include "Components/ArrowComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/BoxComponent.h"
#include "KMK_Player.h"
#include "KMK_PlayerHand.h"
#include "KMK_PlayerHandFSM.h"


// Sets default values
AKMK_PlayerHand::AKMK_PlayerHand()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//// 왼손
	hand = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HandComp"));
	SetRootComponent(hand);
	hand->SetRelativeRotation(FRotator(0, -90, 0));
	hand->SetRelativeScale3D(FVector(1.5f));
	hand->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// 발사 위치만들기
	arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("FirePos"));
	arrow->SetupAttachment(hand);
	arrow->SetRelativeLocationAndRotation(FVector(0, 10, 2.2f), FRotator(0, 90, 0));
	arrow->SetRelativeScale3D(FVector(0.2f));
	// 콜리전
	box = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	box->SetupAttachment(RootComponent);
	box->SetCollisionProfileName("Hand");
	box->SetRelativeLocation(FVector(0, 2, 4));
	box->SetBoxExtent(FVector(5, 2, 5));
	// 물건을 잡기 위한 handle
	handle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("Handle"));

}

// Called when the game starts or when spawned
void AKMK_PlayerHand::BeginPlay()
{
	Super::BeginPlay();
	player = Cast<AKMK_Player>(GetWorld()->GetFirstPlayerController()->GetPawn());
	hand->SetStaticMesh(HandMesh[0]);
	box->OnComponentBeginOverlap.AddDynamic(this, &AKMK_PlayerHand::BeginOverlap);
	// FSM = player->FSM;
}

// Called every frame
void AKMK_PlayerHand::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (t > ShootTime && !isRay)
	{
		isGo = false;
		isReverse = true;
	}
	if (isGo)
	{
		t += DeltaTime;
		dir = endPos - GetActorLocation();
		float distance = dir.Length();
		dir.Normalize();
		SetActorLocation(GetActorLocation() + dir * speed * DeltaTime);
		
		if(distance < 5)
		{
			isGo = false;
			isReverse = true;
		}
		
	}
	if (isReverse)
	{
		t= 0;
		dir = startPos - GetActorLocation();
		float distance = dir.Length();
		dir.Normalize();
		SetActorLocation(GetActorLocation() + dir * speed / 2 * DeltaTime);
		if (distance < 5.f)
		{
			isReverse = false;
			isRay = false;
			SetActorRelativeLocation(FVector(30, handPos, -13));
		}
	}
	
}

// 손이 다른 물체들과 닿는 경우
void AKMK_PlayerHand::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetActorLabel().Contains("jump"))
	{
		FSM->isJump = true;
		FSM->PState = PlayerHandFSM::JumpPack;
	}
	if (player->RMeshComp->GetStaticMesh() == player->RHand->HandMesh[0] && OtherActor->GetActorLabel().Contains("ElectricalPanel"))
	{
		FSM->PState = PlayerHandFSM::Energy;
		FSM->t = 0;
		FSM->isCharge = true;
	}
	if (OtherActor->GetActorLabel().Contains("battery") && player->RMeshComp->GetStaticMesh() != player->RHand->HandMesh[2])
	{
		OtherActor->AttachToComponent(hand, FAttachmentTransformRules::KeepWorldTransform);
	}
	isGo = false;
	isReverse = true;
}
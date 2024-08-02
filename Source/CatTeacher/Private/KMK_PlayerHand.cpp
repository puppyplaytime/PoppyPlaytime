// Fill out your copyright notice in the Description page of Project Settings.

#include "KMK_PlayerHand.h"
#include "../../../../Plugins/Runtime/CableComponent/Source/CableComponent/Classes/CableComponent.h"
#include "Components/ArrowComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/BoxComponent.h"
#include "KMK_Player.h"
#include "KMK_PlayerHandFSM.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/MovementComponent.h"
#include "KMK_Battery.h"
#include "Engine/HitResult.h"
#include "Components/PrimitiveComponent.h"
#include "KMK_Bat.h"
// Sets default values
AKMK_PlayerHand::AKMK_PlayerHand()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//// 왼손
	hand = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HandComp"));
	SetRootComponent(hand);
	hand->SetRelativeRotation(FRotator(0, -90, 0));
	hand->SetRelativeScale3D(FVector(2.f));
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
	box->SetRelativeLocation(FVector(0, 2, 2));
	box->SetBoxExtent(FVector(0.5f, 0.375, 0.5f));
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
	box->BodyInstance.bUseCCD = true;
	box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//box->OnComponentHit.AddDynamic(this, &AKMK_PlayerHand::OnHitEvent);
	// FSM = player->FSM;
}

// Called every frame
void AKMK_PlayerHand::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
#pragma region HandMove
	// 손이 돌아오는 코드
	if (isReverse)
	{
		// 방향 = 목적지(손의 원위치) - 손의 위치
		dir = startPos - GetActorLocation();
		// 거리측정
		float d = dir.Length();
		// GEngine->AddOnScreenDebugMessage(10, 1, FColor::White, FString::Printf(TEXT("%f"), d));
		dir.Normalize();
		// 이동
		SetActorLocation(GetActorLocation() + dir * speed * DeltaTime);
		// ray거리보다 크거나, 목적지에 어느정도 도착했다면
		if (d < 40.f || d  > player->rayDis)
		{
			// 1. 시간 초기화
			t = 0;
			// 2. 돌아오는 코드 정지
			isReverse = false;
			// 3. 레이 못쏘게 만들기
			isRay = false;
			// 손의 원위치 및 콜라이더 끄기
			SetActorRelativeLocation(FVector(40, handPos, -16));
			box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
	// 손이 나가는 부분
	if (isGo)
	{
		// 손에 배터리가 없는 경우에 손을 뻗으며 콜리전을 켜줌
		if (!isGrab)box->SetCollisionProfileName("Hand");
		// 일정 시간이 지나거나, 레이를 쏘지 않거나, 배터리가 없는 경우에
		if (t > ShootTime && !isRay && !isGrab)
		{
			// 손이 가는 것을 멈추고 돌아오게 만듦
			isGo = false;
			isReverse = true;
		}
		// 시간 추가
		t += DeltaTime;
		// 나아가는 방향 = 목적지(레이 위치) - 내 위치
		dir = endPos - GetActorLocation();
		// 거리 측정 및 이동
		float distance = dir.Length();
		dir.Normalize();
		SetActorLocation(GetActorLocation() + dir * speed * DeltaTime);
		// 레이 거리보다 멀게 나가거나 목적지에 거의 도착하면
		if (distance > player->rayDis || distance < 5)
		{
			// 돌아오게 만들기
			isGo = false;
			isReverse = true;
		}

	}
#pragma endregion


#pragma region Battery Grab
	// 배터리를 잡는 상태인 경우
	if (isGrab)
	{
		// 손의 콜라이더를 꺼주고
		box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		// 오른손의 입력값이 들어온다면(재클릭)
		if (player->isRight)
		{
			// 오른손에 배터리를 들고있을 경우에만 배터리 생성하기 위함
			if (isLeft) return;
			// 오른손 배터리의 위치를 저장하고
			trans = player->RBat->GetTransform();
			// 오른손 배터리가 안 보이게 만들어줌
			player->RBat->meshComp->SetVisibility(false);
			// 배터리가 슬롯에 들어가지 않았다면, 월드에 배터리를 생성해줌
			if (!player->RBat->isPut)GetWorld()->SpawnActor<AKMK_Battery>(BatteryFact, trans);
			else
			{
				player->RBat->bat->isCome = true;
				// 배터리가 슬롯에 들어간 상태임
				isGrab = false;
				isRight = false;
			}
		}
		// 왼손인 경우
		if (player->isLeft)
		{
			// 왼손에 배터리를 들고있을 경우에만 배터리 생성하기 위함
			if (isRight) return;
			// 위와 같은 로직
			trans = player->LBat->GetTransform();
			player->LBat->meshComp->SetVisibility(false);
			
			if (!player->LBat->isPut)GetWorld()->SpawnActor<AKMK_Battery>(BatteryFact, trans);
			else
			{
				player->LBat->bat->isCome = true;
				isGrab = false;
				isLeft = false;
				
			}
		}
	}
	else
	{	
		if(!isCome) return;
		if (player->isRight)
		{
			player->RBat->meshComp->SetVisibility(true);
			isGrab = true;
			b->isCome = false;
			player->RBat->isPut = false;
		}
		if (player->isLeft)
		{
			player->LBat->meshComp->SetVisibility(true);
			isGrab = true;
			b->isCome = false;
			player->LBat->isPut = false;
		}
	}

#pragma endregion

	if (isPick)
	{
		hand->SetWorldLocation(pickTrans);
	}

}

// 손이 다른 물체들과 닿는 경우
void AKMK_PlayerHand::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 물체가 닿은 경우에 손이 돌아오게 만들기 위함
	isGo = false;
	isReverse = true;

	GEngine->AddOnScreenDebugMessage(9, 1, FColor::White, FString::Printf(TEXT("%s"), *OtherActor->GetName()));
	if (OtherActor->GetActorLabel().Contains("MK"))
	{
		b = OtherActor->FindComponentByClass<UKMK_Bat>();
		if (b->isCome)
		{
			isCome = true;
		}
	}
	// 배터리가 손에 닿은 경우
	if (OtherActor->ActorHasTag("Battery"))
	{
		// grabActor에 할당
		grabActor = Cast<AKMK_Battery>(OtherActor);
		// 이미 배터리를 잡고 있는 경우에는 반환
		if(isGrab) return;

		// 오른손인 경우에
		if (GetName().Contains("R"))
		{
			// 총이 아닌 상태
			if (player->RMeshComp->GetStaticMesh() != player->RHand->HandMesh[2])
			{
				// 잡은 상태로 변경
				isGrab = true;
				// 닿은 배터리는 제거
				grabActor->Destroy();
				// 보이지 않은 상태에 들고있는 배터리를 보여주고 콜리전을 켜줌
				player->RBat->meshComp->SetVisibility(true);
				player->RBat->meshComp->SetCollisionProfileName("Item");
				isRight = true;
			}
		}
		// 왼손인 경우
		else
		{
			// 위와 같은 상태, 단 왼손의 경우, 변경사항이 없기에 한번에 처리함
			isGrab = true;
			isLeft = true;
			grabActor->Destroy();
			player->LBat->meshComp->SetVisibility(true);
			player->LBat->meshComp->SetCollisionProfileName("Item");
		}
	}
	// 잡을 수 있는 오브젝트에 닿은 경우
	if (OtherComp->ComponentHasTag("Handle"))
	{
		// 평범한 손이 아니면 반환
		if (player->RMeshComp->GetStaticMesh() != player->RHand->HandMesh[0]) return;
		// 위치값을 받아와 위치에 넣어줌
		pickTrans = OtherComp->GetChildComponent(0)->GetComponentLocation();
		isPick = true;
	}
	// 왼손인 상태면 밑에 상황이 필요 없음 => 반환
	if(!GetName().Contains("R")) return;	
	// 점프손인 경우에, 점프 패드가 닿는다면
	if (player->RMeshComp->GetStaticMesh() == player->RHand->HandMesh[2] && OtherActor->ActorHasTag("Jump"))
	{
		// 플레이어의 상태를 변경
		FSM->isJump = true;
		FSM->PState = PlayerHandFSM::JumpPack;
	}
	// 평범한 손이고, panel에 닿는다면
	if (player->RMeshComp->GetStaticMesh() == player->RHand->HandMesh[0] && OtherActor->ActorHasTag("ElectricalPanel"))
	{
		// 충전 상태로 변경
		FSM->PState = PlayerHandFSM::Energy;
		FSM->t = 0;
		FSM->isCharge = true;
	}


}



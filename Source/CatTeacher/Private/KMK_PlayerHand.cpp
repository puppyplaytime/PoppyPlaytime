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
	if (isReverse)
	{
		dir = startPos - GetActorLocation();
		float d = dir.Length();
		// GEngine->AddOnScreenDebugMessage(10, 1, FColor::White, FString::Printf(TEXT("%f"), d));
		dir.Normalize();
		SetActorLocation(GetActorLocation() + dir * speed * DeltaTime);
		if (d < 40.f || d  > player->rayDis)
		{
			t = 0;
			isReverse = false;
			isRay = false;
			SetActorRelativeLocation(FVector(40, handPos, -16));
			box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
	if (isGo)
	{
		if(!isGrab)box->SetCollisionProfileName("Hand");
		if (t > ShootTime && !isRay && !isGrab)
		{
			isGo = false;
			isReverse = true;
		}
		t += DeltaTime;
		dir = endPos - GetActorLocation();
		float distance = dir.Length();
		dir.Normalize();
		SetActorLocation(GetActorLocation() + dir * speed * DeltaTime);
		if(distance > player->rayDis || distance < 5)
		{
			isGo = false;
			isReverse = true;
		}
		
	}

	if (isGrab)
	{
		box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		if (player->isRight)
		{
			if (isLeft) return;
			trans = player->RBat->GetTransform();
			player->RBat->meshComp->SetVisibility(false);
			if(!player->RBat->isPut)GetWorld()->SpawnActor<AKMK_Battery>(BatteryFact, trans);
			else 
			{
				isGrab = false; 
				isRight = false;
			}
		}
		if (player->isLeft)
		{
			if(isRight) return;
			trans = player->LBat->GetTransform();
			player->LBat->meshComp->SetVisibility(false);
			if (!player->LBat->isPut)GetWorld()->SpawnActor<AKMK_Battery>(BatteryFact, trans);
			else
			{
				isGrab = false;
				isLeft = false;
			}
		}
	}

	if (isPick)
	{
		hand->SetWorldLocation(pickTrans);
	}

}

// 손이 다른 물체들과 닿는 경우
void AKMK_PlayerHand::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	isGo = false;
	isReverse = true;

	//GEngine->AddOnScreenDebugMessage(9, 1, FColor::White, FString::Printf(TEXT("%s"), *OtherActor->GetName()));
	GEngine->AddOnScreenDebugMessage(9, 1, FColor::White, FString::Printf(TEXT("%s"), *OtherActor->GetName()));
	if (OtherActor->ActorHasTag("Battery"))
	{

		grabActor = Cast<AKMK_Battery>(OtherActor);
		if(isGrab) return;
		if (GetName().Contains("R"))
		{
			if (player->RMeshComp->GetStaticMesh() != player->RHand->HandMesh[2])
			{
				isGrab = true;
				grabActor->Destroy();
				player->RBat->meshComp->SetVisibility(true);
				player->RBat->meshComp->SetCollisionProfileName("Item");
				isRight = true;
			}
		}
		else
		{
			isGrab = true;
			isLeft = true;
			grabActor->Destroy();
			player->LBat->meshComp->SetVisibility(true);
			player->LBat->meshComp->SetCollisionProfileName("Item");

		}

	}
	if (player->RMeshComp->GetStaticMesh() == player->RHand->HandMesh[2] && OtherActor->ActorHasTag("Jump"))
	{
		FSM->isJump = true;
		FSM->PState = PlayerHandFSM::JumpPack;
	}
	if (player->RMeshComp->GetStaticMesh() == player->RHand->HandMesh[0] && OtherActor->ActorHasTag("ElectricalPanel"))
	{
		FSM->PState = PlayerHandFSM::Energy;
		FSM->t = 0;
		FSM->isCharge = true;
	}
	if (OtherComp->ComponentHasTag("Handle"))
	{
		if(player->RMeshComp->GetStaticMesh() == player->RHand->HandMesh[2]) return;	
		pickTrans = OtherComp->GetChildComponent(0)->GetComponentLocation();
		isPick = true;
	}
}



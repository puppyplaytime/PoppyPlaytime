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
	//// �޼�
	hand = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HandComp"));
	SetRootComponent(hand);
	hand->SetRelativeRotation(FRotator(0, -90, 0));
	hand->SetRelativeScale3D(FVector(2.f));
	hand->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// �߻� ��ġ�����
	arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("FirePos"));
	arrow->SetupAttachment(hand);
	arrow->SetRelativeLocationAndRotation(FVector(0, 10, 2.2f), FRotator(0, 90, 0));
	arrow->SetRelativeScale3D(FVector(0.2f));
	// �ݸ���
	box = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	box->SetupAttachment(RootComponent);
	box->SetCollisionProfileName("Hand");
	box->SetRelativeLocation(FVector(0, 2, 2));
	box->SetBoxExtent(FVector(0.5f, 0.375, 0.5f));
	// ������ ��� ���� handle
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
	// ���� ���ƿ��� �ڵ�
	if (isReverse)
	{
		// ���� = ������(���� ����ġ) - ���� ��ġ
		dir = startPos - GetActorLocation();
		// �Ÿ�����
		float d = dir.Length();
		// GEngine->AddOnScreenDebugMessage(10, 1, FColor::White, FString::Printf(TEXT("%f"), d));
		dir.Normalize();
		// �̵�
		SetActorLocation(GetActorLocation() + dir * speed * DeltaTime);
		// ray�Ÿ����� ũ�ų�, �������� ������� �����ߴٸ�
		if (d < 40.f || d  > player->rayDis)
		{
			// 1. �ð� �ʱ�ȭ
			t = 0;
			// 2. ���ƿ��� �ڵ� ����
			isReverse = false;
			// 3. ���� ����� �����
			isRay = false;
			// ���� ����ġ �� �ݶ��̴� ����
			SetActorRelativeLocation(FVector(40, handPos, -16));
			box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
	// ���� ������ �κ�
	if (isGo)
	{
		// �տ� ���͸��� ���� ��쿡 ���� ������ �ݸ����� ����
		if (!isGrab)box->SetCollisionProfileName("Hand");
		// ���� �ð��� �����ų�, ���̸� ���� �ʰų�, ���͸��� ���� ��쿡
		if (t > ShootTime && !isRay && !isGrab)
		{
			// ���� ���� ���� ���߰� ���ƿ��� ����
			isGo = false;
			isReverse = true;
		}
		// �ð� �߰�
		t += DeltaTime;
		// ���ư��� ���� = ������(���� ��ġ) - �� ��ġ
		dir = endPos - GetActorLocation();
		// �Ÿ� ���� �� �̵�
		float distance = dir.Length();
		dir.Normalize();
		SetActorLocation(GetActorLocation() + dir * speed * DeltaTime);
		// ���� �Ÿ����� �ְ� �����ų� �������� ���� �����ϸ�
		if (distance > player->rayDis || distance < 5)
		{
			// ���ƿ��� �����
			isGo = false;
			isReverse = true;
		}

	}
#pragma endregion


#pragma region Battery Grab
	// ���͸��� ��� ������ ���
	if (isGrab)
	{
		// ���� �ݶ��̴��� ���ְ�
		box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		// �������� �Է°��� ���´ٸ�(��Ŭ��)
		if (player->isRight)
		{
			// �����տ� ���͸��� ������� ��쿡�� ���͸� �����ϱ� ����
			if (isLeft) return;
			// ������ ���͸��� ��ġ�� �����ϰ�
			trans = player->RBat->GetTransform();
			// ������ ���͸��� �� ���̰� �������
			player->RBat->meshComp->SetVisibility(false);
			// ���͸��� ���Կ� ���� �ʾҴٸ�, ���忡 ���͸��� ��������
			if (!player->RBat->isPut)GetWorld()->SpawnActor<AKMK_Battery>(BatteryFact, trans);
			else
			{
				player->RBat->bat->isCome = true;
				// ���͸��� ���Կ� �� ������
				isGrab = false;
				isRight = false;
			}
		}
		// �޼��� ���
		if (player->isLeft)
		{
			// �޼տ� ���͸��� ������� ��쿡�� ���͸� �����ϱ� ����
			if (isRight) return;
			// ���� ���� ����
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

// ���� �ٸ� ��ü��� ��� ���
void AKMK_PlayerHand::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// ��ü�� ���� ��쿡 ���� ���ƿ��� ����� ����
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
	// ���͸��� �տ� ���� ���
	if (OtherActor->ActorHasTag("Battery"))
	{
		// grabActor�� �Ҵ�
		grabActor = Cast<AKMK_Battery>(OtherActor);
		// �̹� ���͸��� ��� �ִ� ��쿡�� ��ȯ
		if(isGrab) return;

		// �������� ��쿡
		if (GetName().Contains("R"))
		{
			// ���� �ƴ� ����
			if (player->RMeshComp->GetStaticMesh() != player->RHand->HandMesh[2])
			{
				// ���� ���·� ����
				isGrab = true;
				// ���� ���͸��� ����
				grabActor->Destroy();
				// ������ ���� ���¿� ����ִ� ���͸��� �����ְ� �ݸ����� ����
				player->RBat->meshComp->SetVisibility(true);
				player->RBat->meshComp->SetCollisionProfileName("Item");
				isRight = true;
			}
		}
		// �޼��� ���
		else
		{
			// ���� ���� ����, �� �޼��� ���, ��������� ���⿡ �ѹ��� ó����
			isGrab = true;
			isLeft = true;
			grabActor->Destroy();
			player->LBat->meshComp->SetVisibility(true);
			player->LBat->meshComp->SetCollisionProfileName("Item");
		}
	}
	// ���� �� �ִ� ������Ʈ�� ���� ���
	if (OtherComp->ComponentHasTag("Handle"))
	{
		// ����� ���� �ƴϸ� ��ȯ
		if (player->RMeshComp->GetStaticMesh() != player->RHand->HandMesh[0]) return;
		// ��ġ���� �޾ƿ� ��ġ�� �־���
		pickTrans = OtherComp->GetChildComponent(0)->GetComponentLocation();
		isPick = true;
	}
	// �޼��� ���¸� �ؿ� ��Ȳ�� �ʿ� ���� => ��ȯ
	if(!GetName().Contains("R")) return;	
	// �������� ��쿡, ���� �е尡 ��´ٸ�
	if (player->RMeshComp->GetStaticMesh() == player->RHand->HandMesh[2] && OtherActor->ActorHasTag("Jump"))
	{
		// �÷��̾��� ���¸� ����
		FSM->isJump = true;
		FSM->PState = PlayerHandFSM::JumpPack;
	}
	// ����� ���̰�, panel�� ��´ٸ�
	if (player->RMeshComp->GetStaticMesh() == player->RHand->HandMesh[0] && OtherActor->ActorHasTag("ElectricalPanel"))
	{
		// ���� ���·� ����
		FSM->PState = PlayerHandFSM::Energy;
		FSM->t = 0;
		FSM->isCharge = true;
	}


}



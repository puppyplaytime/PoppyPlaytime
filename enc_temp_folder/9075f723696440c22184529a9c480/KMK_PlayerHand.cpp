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
#include "KHH_BossOpendoor.h"
#include "PlayerAnimInstance.h"
#include "KHH_RotateDoor.h"
#include "PlayerWidget.h"
#include "../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h"
#include "../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h"
#include <LeverComponent.h>
#include "KMK_FinalSwitch.h"
// Sets default values
AKMK_PlayerHand::AKMK_PlayerHand()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
#pragma region Creaate
	//// �޼�
	hand = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HandComp"));
	SetRootComponent(hand);
	hand->SetRelativeRotation(FRotator(0, -90, 0));
	hand->SetRelativeScale3D(FVector(2.f));
	hand->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// �߻� ��ġ������
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
	// ������ ���� ���� handle
	handle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("Handle"));
#pragma endregion

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
	VFXComp = FindComponentByClass< UNiagaraComponent>();
	if(VFXComp) VFXComp->SetVisibility(false);
	// ���׸��� ����
	myMatDynamic = UMaterialInstanceDynamic::Create(matFact, this);
	if(matFact != nullptr) hand->SetMaterial(0, myMatDynamic);
	myMatDynamic->SetScalarParameterValue("charge_light", 0);
}

// Called every frame
void AKMK_PlayerHand::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	for (int i = 0; i < 3; i++)
	{
		if (player->isClick[i] && !player->bChangeHand)
		{
			return;
		}
	}

#pragma region isPick
	// ������ ���� ���쿡 ��ġ ����
	if (isPick)
	{
		if(lever != nullptr && !lever->LeverMove) isPick = false;
		GEngine->AddOnScreenDebugMessage(11, 5, FColor::Purple, FString::Printf(TEXT("%s"), *pickTrans.ToString()));
		box->SetCollisionProfileName("Hand");
		hand->SetWorldLocation(pickTrans);
	}
	// ����ġ ���� �ð�
	if (isHold)
	{
		holdTime += DeltaTime;
		if (holdTime > HandTime)
		{
			holdTime = 0;
			isHold = false;
		}
	}
#pragma endregion

	// hatch������ �ݰ� �ϴ� �κ�
	if (isDoor)
	{
		// �� �ݱ�
		if (isPick)
		{
			rotDoor->isOpen = false;
			if (GetName().Contains("R")) rotDoor->isRight = true;
			else rotDoor->isLeft = true;
			rot = FRotator(-50, 0, 0);
			rotDoor->RotateDoor1(DeltaTime, rot, rotDoor->MoveTime);
			// ���� �� ���� ����, ����� ���ǹ�
			if (rotDoor->GetOwner()->GetActorRotation().Pitch < 0)
			{
				// �ؿ� �� ������ false �̸� ��ġ ������ ����
				isClosed = false;
				isDie = false;

				// �Ű� X
				isPick = false;
				isDoor = false;
				rotDoor->isOpen = false;
				rotDoor->isRight = false;
				rotDoor->isLeft = false;
				rotDoor->cnt = 0;
			}
		}
		// ������
		else
		{
			if (GetName().Contains("R")) rotDoor->isRight = false;
			else rotDoor->isLeft = false;
			rotDoor->isOpen = true;
			// ��ġ�κ� ������ ���۵��� �˷���
			isClosed = true;
			// ���� �� ���� ������ ����� ���ǹ�
			if (rotDoor->GetOwner()->GetActorRotation().Pitch > 50)
			{
				// ��ġ�κ� ������ ������ ��Ŵ
				isClosed = false;
				// �÷��̾ �׾����ϴ� Ÿ�̹� bool����
				isDie = true;
				// �Ű澲��������
				isPick = false;
				isDoor = false;
				rotDoor->isOpen = false;
				rotDoor->isRight = false;
				rotDoor->isLeft = false;
				rotDoor->cnt = 0;
			}
		}
	}

#pragma region HandMove
	if (isReverse)
	{
		isGo = false;
	}
	if (isGo)
	{
		isReverse = false;
	}
	// ���� ���ƿ��� �ڵ�
	if (isReverse && !isGo)
	{
		if(isHold) return;
		player->anim->PlayHandInMontage();
		// ���� = ������(���� ����ġ) - ���� ��ġ
		dir = startPos - GetActorLocation();
		// �Ÿ�����
		float d = dir.Length();
		// GEngine->AddOnScreenDebugMessage(10, 1, FColor::White, FString::Printf(TEXT("%f"), d));
		dir.Normalize();
		// �̵�
		SetActorLocation(GetActorLocation() + dir * speed * DeltaTime);
		// ray�Ÿ����� ũ�ų�, �������� �������� �����ߴٸ�
		if (d < 40.f)
		{
			// 1. �ð� �ʱ�ȭ
			t = 0;
			// 2. ���ƿ��� �ڵ� ����
			isReverse = false;
			// 3. ���� ������ ������
			isRay = false;
			for (int i = 0; i < 2; i++)
			{
				player->CableComp[i]->SetRenderInMainPass(false);
			}
			// ���� ����ġ �� �ݶ��̴� ����
			SetActorRelativeLocation(FVector(0));
			box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}

	// ���� ������ �κ�
	if (isGo && !isReverse)
	{
		for (int i = 0; i < 2; i++)
		{
			player->CableComp[i]->SetRenderInMainPass(true);
		}
		player->anim->PlayHandMontage();
		// �տ� ���͸��� ���� ���쿡 ���� ������ �ݸ����� ����
		if (!isGrab)box->SetCollisionProfileName("Hand");
		// ���� �ð��� �����ų�, ���̸� ���� �ʰų�, ���͸��� ���� ���쿡
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
		if (distance > player->rayDis || distance < 40)
		{
			// ���ƿ��� ������
			isGo = false;
			isReverse = true;
		}

	}
#pragma endregion
	GEngine->AddOnScreenDebugMessage(3, 1, FColor::Orange, FString::Printf(TEXT("%d"), isCome));
#pragma region Battery Grab
	//
	// ���͸��� ���� ������ ����
	if (isGrab )
	{
		// ���� �ݶ��̴��� ���ְ�
		box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		// �������� �Է°��� �����´ٸ�(��Ŭ��)
		if (player->isDir[0])
		{
			// �����տ� ���͸��� �������� ���쿡�� ���͸� �����ϱ� ����
			if (player->isDir[1] || player->Hands[1]->isGrab)
			{
				return;
			}
			MakeBattery(0);

		}
		// �޼��� ����
		if (player->isDir[1])
		{
			// �޼տ� ���͸��� �������� ���쿡�� ���͸� �����ϱ� ����
			if (player->isDir[0] || player->Hands[0]->isGrab) return;
			// ���� ���� ����
			MakeBattery(1);
		}
	}

#pragma endregion



}

// ���� �ٸ� ��ü���� ���� ����
void AKMK_PlayerHand::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	isGo = false;
	isReverse = true;
	// ��ü�� ���� ���쿡 ���� ���ƿ��� ������ ����
     if (bFromSweep)
     {
         isGo = false;
         isReverse = true;
     }

	// BeginOverlap�� ������ �̸��� �����ϴ� ���� (����ġ ��)
	SwitchName = *OtherActor->GetName();
	// BeginOverlap�� ���͸� �����ϴ� ���� ( ����ġ�� �̰ɷ� �ٲٸ� �� ��)
	overActor = OtherActor;

	// ���͸��� �տ� ���� ����
	if (OtherActor->ActorHasTag("Battery"))
	{
		// ������ ���� �ƴϸ� ��ȯ
		if (player->RMeshComp->GetStaticMesh() != player->Hands[0]->HandMesh[0]) return;
		// grabActor�� �Ҵ�
		grabActor = Cast<AKMK_Battery>(OtherActor);
		// �̹� ���͸��� ���� �ִ� ���쿡�� ��ȯ
		if (isGrab) return;
		// ���� ���·� ����
		isGrab = true;
		// ���� ���͸��� ����
		grabActor->Destroy();
		// �������� ���쿡
		if (GetName().Contains("R"))
		{
			// ���� �ƴ� ����
			if (player->RMeshComp->GetStaticMesh() != player->Hands[0]->HandMesh[2])
			{
				// ������ ���� ���¿� �����ִ� ���͸��� �����ְ� �ݸ����� ����
				player->Bats[0]->meshComp->SetRenderInMainPass(true);
				player->Bats[0]->meshComp->SetCollisionProfileName("Bat");
				isRight = true;
			}
		}
		// �޼��� ����
		else
		{
			// ���� ���� ����, �� �޼��� ����, ���������� ���⿡ �ѹ��� ó����
			isLeft = true;
			player->Bats[1]->meshComp->SetRenderInMainPass(true);
			player->Bats[1]->meshComp->SetCollisionProfileName("Bat");
		}
	}
	// ���� �� �ִ� ������Ʈ�� ���� ����
	if (OtherComp->ComponentHasTag("Handle"))
	{
		// ������ ���� �ƴϸ� ��ȯ
		if (player->RMeshComp->GetStaticMesh() != player->Hands[0]->HandMesh[0]) return;
		// ��ġ���� �޾ƿ� ��ġ�� �־���
		pickTrans = OtherComp->GetChildComponent(0)->GetComponentLocation();

		isPick = true;
		if(OtherActor->FindComponentByClass<UKHH_BossOpendoor>() != nullptr)OtherActor->FindComponentByClass<UKHH_BossOpendoor>()->ShouldMove = true;
		if (OtherActor->FindComponentByClass<UKHH_RotateDoor>() != nullptr)
		{
			firstRot = OtherActor->GetActorRotation();
			rotDoor = OtherActor->FindComponentByClass<UKHH_RotateDoor>();
			isDoor = true;
		}
		lever = OtherActor->FindComponentByClass<ULeverComponent>();
		if(lever != nullptr)
		{
			leverRot = OtherActor->GetActorRotation();
			lever->LeverMove = true;
			isLever = true;
		}
	}
	// �޼��� ���¸� �ؿ� ��Ȳ�� �ʿ� ���� => ��ȯ
	// �������� ���쿡, ���� �е尡 ���´ٸ�
	if (isJump && GetName().Contains("R"))
	{
		if (OtherActor->ActorHasTag("Jump") && player->RMeshComp->GetStaticMesh() == player->Hands[0]->HandMesh[2])
		{
			// �÷��̾��� ���¸� ����
			FSM->isJump = true;
			isJump = false;
			FSM->PState = PlayerHandFSM::JumpPack;
		}
	}
	else
	{
			box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// ������ ���̰�, panel�� ���´ٸ�
	if (player->RMeshComp->GetStaticMesh() == player->Hands[0]->HandMesh[0] && OtherActor->ActorHasTag("ElectricalPanel") && GetName().Contains("R"))
	{
		// ���� ���·� ����
		FSM->PState = PlayerHandFSM::Energy;
		FSM->t = 0;
		FSM->isCharge = true;
		myMatDynamic->SetScalarParameterValue("charge_light", 4);
		VFXComp->SetVisibility(true);
		isHold = true;
		// �̰��� ������ �ִϸ��̼� ��ŸƮ �κ� �����ø� �˴ϴ�.
		if (OtherActor->FindComponentByClass<UKMK_FinalSwitch>())
		{

			GEngine->AddOnScreenDebugMessage(9, 1, FColor::Emerald, FString::Printf(TEXT("complete")));
		}
	}

}

void AKMK_PlayerHand::MakeBattery(int32 num)
{
	// ������ ���͸��� ��ġ�� �����ϰ�
	trans = player->Bats[num]->GetTransform();
	// ������ ���͸��� �� ���̰� ��������
	player->Bats[num]->SetVis(false);

	// ���͸��� ���Կ� ����� �ʾҴٸ�, ���忡 ���͸��� ��������
	if (!isCome && !isPick)
	{
		GetWorld()->SpawnActor<AKMK_Battery>(BatteryFact, trans);
	}
}

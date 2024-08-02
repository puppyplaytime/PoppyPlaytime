// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK_PlayerRay.h"
#include "KMK_PlayerHandFSM.h"
#include "KMK_Player.h"
#include "KMK_PlayerHand.h"
#include "../../../../Plugins/Runtime/CableComponent/Source/CableComponent/Classes/CableComponent.h"
#include "KHH_EnemyFSM.h"
#include "Components/ArrowComponent.h"
#include "KMK_Bat.h"
#include "KMK_Battery.h"

// Sets default values for this component's properties
UKMK_PlayerRay::UKMK_PlayerRay()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UKMK_PlayerRay::BeginPlay()
{
	Super::BeginPlay();
	FSM = GetOwner()->FindComponentByClass<UKMK_PlayerHandFSM>();
	playerComp = Cast<AKMK_Player>(GetOwner());
}


// Called every frame
void UKMK_PlayerRay::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ���� ����
	FCollisionQueryParams params;
	params.AddIgnoredActor(GetOwner());
	params.AddIgnoredActor(playerComp->Hands[0]);
	params.AddIgnoredActor(playerComp->Hands[1]);
	// ���� ��� ������ �κ�
	endPos = playerComp->endPos;

	// hit�� ��ü ���� ������
	FHitResult hitInfo;

	// ���̸� ���
	bool bhit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, params);
	//DrawDebugLine(GetWorld(), playerComp->startPos, endPos, FColor::Red, false, 1.f);
	// Ŭ�������� ���´ٸ�,

	if (bhit && hitInfo.GetActor()->ActorHasTag("E_Bat"))
	{
		playerComp->RBat->bat = hitInfo.GetActor()->FindComponentByClass<UKMK_Bat>();
		playerComp->LBat->bat = hitInfo.GetActor()->FindComponentByClass<UKMK_Bat>();
		b = hitInfo.GetActor()->FindComponentByClass<UKMK_Bat>();
		if (!b->isCome && (!playerComp->RHand->isGrab && !playerComp->LHand->isGrab))
		{
			if(!playerComp->RBat->isPut)playerComp->RBat->isPut = true;
			if(!playerComp->LBat->isPut)playerComp->LBat->isPut = true;
		}

	}
	if (isRay)
	{
		// ���̸� �׸���
		DrawDebugLine(GetWorld(), startPos, endPos, FColor::Blue, false, 1.f);
		cnt++;
		//  ������ �̶��
		if(playerComp->isRight)
		{
			// ���̿� ��ü�� ���� ���
			if(bhit)
			{
				// ���� ��� �յ���
				if (playerComp->RMeshComp->GetStaticMesh() == playerComp->Hands[0]->HandMesh[1])
				{
					// ���� ���
					FSM->isFire	= true;
					// FSM�� gunpack���� �����ϰ�
					FSM->PState = PlayerHandFSM::GunPack;
				}
				// ���� ������� �ʴٸ�
				else
				{
					// �����տ� ��ü�� component�� �Ѱ��ְ�
					playerComp->Hands[0]->hitinfo = hitInfo.GetComponent();
					// ���� ���� �����
					// �� �ʱⰪ
					playerComp->Hands[0]->handPos = 27;
					// ���� ��������
					playerComp->Hands[0]->endPos = hitInfo.ImpactPoint;
					// ���� ���� ����
					if (playerComp->RHand->isCome)
					{
						playerComp->RHand->isCome = false;
						playerComp->RBat->isPut = false;
						playerComp->RHand->isGrab = true;
						return;
					}
					if (playerComp->RHand->isGrab || playerComp->RHand->isPick)
					{
						playerComp->Hands[0]->isGrab = false;
						playerComp->Hands[0]->isPick = false;
					}
					else playerComp->Hands[0]->isGo = true;
				}
			}
			// ��ü�� ���� ���� ���
			else
			{
				if (playerComp->Hands[0]->isGrab)
				{
					playerComp->Hands[0]->isGrab = false;
					return;
				}
				// 1. ���� ��� ���� �ʴٸ�
				if (playerComp->RMeshComp->GetStaticMesh() != playerComp->Hands[0]->HandMesh[1])
				{
					// ���� ���´�
					playerComp->Hands[0]->handPos = 27;
					playerComp->Hands[0]->endPos = playerComp->endPos;
					playerComp->Hands[0]->isGo = true;
				}
				// �ƴ϶��
				else
				{
					// �ѽ��
					FSM->isFire = true;
				}
			}

		}
		// �޼��̶��
		if (playerComp->isLeft)
		{
			if (playerComp->Hands[1]->isGrab)
			{
				playerComp->Hands[1]->isGrab = false;
				return;
			}
			// ��ü�� �ִٸ�
			if (bhit)
			{
				playerComp->Hands[1]->endPos = hitInfo.ImpactPoint;
				playerComp->Hands[1]->hitinfo = hitInfo.GetComponent();
			}
			else playerComp->Hands[1]->endPos = playerComp->endPos;
			// ���������� ����
			playerComp->Hands[1]->handPos = -27;
			playerComp->Hands[1]->isRay = true;
			if (playerComp->Hands[1]->isGrab || playerComp->Hands[1]->isPick)
			{
				playerComp->Hands[1]->isGrab = false;
				playerComp->Hands[1]->isPick = false;
			}
			else playerComp->Hands[1]->isGo = true;
		}
	}
}

void UKMK_PlayerRay::SetRayPos(FVector start, FVector end)
{
	startPos = start;
	endPos = end;

	isRay = true;
}

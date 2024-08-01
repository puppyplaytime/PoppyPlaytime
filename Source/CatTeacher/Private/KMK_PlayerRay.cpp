// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK_PlayerRay.h"
#include "KMK_PlayerHandFSM.h"
#include "KMK_Player.h"
#include "KMK_PlayerHand.h"
#include "../../../../Plugins/Runtime/CableComponent/Source/CableComponent/Classes/CableComponent.h"
#include "KHH_EnemyFSM.h"
#include "Components/ArrowComponent.h"

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
	params.AddIgnoredActor(playerComp->RHand);
	params.AddIgnoredActor(playerComp->LHand);
	// ���� ��� ������ �κ�
	endPos = playerComp->endPos;

	// hit�� ��ü ���� ������
	FHitResult hitInfo;

	// ���̸� ���
	bool bhit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, params);
	//DrawDebugLine(GetWorld(), playerComp->startPos, endPos, FColor::Red, false, 1.f);
	// Ŭ�������� ���´ٸ�,

	if (isRay)
	{
		// ���̸� �׸���
		DrawDebugLine(GetWorld(), startPos, endPos, FColor::Blue, false, 1.f);
		//  ������ �̶��
		if(playerComp->isRight)
		{
			// ���̿� ��ü�� ���� ���
			if(bhit)
			{
				// ���� ��� �յ���
				if (playerComp->RMeshComp->GetStaticMesh() == playerComp->RHand->HandMesh[1])
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
					playerComp->RHand->hitinfo = hitInfo.GetComponent();
					// ���� ���� �����
					// �� �ʱⰪ
					playerComp->RHand->handPos = 27;
					// ���� ��������
					playerComp->RHand->endPos = hitInfo.ImpactPoint;
					// ���� ���� ����
					if (playerComp->RHand->isGrab || playerComp->RHand->isPick)
					{
						playerComp->RHand->isGrab = false;
						playerComp->RHand->isPick = false;
					}
					else playerComp->RHand->isGo = true;
				}
			}
			// ��ü�� ���� ���� ���
			else
			{
				if (playerComp->RHand->isGrab)
				{
					playerComp->RHand->isGrab = false;
					return;
				}
				// 1. ���� ��� ���� �ʴٸ�
				if (playerComp->RMeshComp->GetStaticMesh() != playerComp->RHand->HandMesh[1])
				{
					// ���� ���´�
					playerComp->RHand->handPos = 27;
					playerComp->RHand->endPos = playerComp->endPos;
					playerComp->RHand->isGo = true;
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
			if (playerComp->LHand->isGrab)
			{
				playerComp->LHand->isGrab = false;
				return;
			}
			// ��ü�� �ִٸ�
			if (bhit)
			{
				playerComp->LHand->endPos = hitInfo.ImpactPoint;
				playerComp->LHand->hitinfo = hitInfo.GetComponent();
			}
			else playerComp->LHand->endPos = playerComp->endPos;
			// ���������� ����
			playerComp->LHand->handPos = -27;
			playerComp->LHand->isRay = true;
			if (playerComp->LHand->isGrab || playerComp->LHand->isPick)
			{
				playerComp->LHand->isGrab = false;
				playerComp->LHand->isPick = false;
			}
			else playerComp->LHand->isGo = true;
		}
	}
}

void UKMK_PlayerRay::SetRayPos(FVector start, FVector end)
{
	startPos = start;
	endPos = end;

	isRay = true;
}


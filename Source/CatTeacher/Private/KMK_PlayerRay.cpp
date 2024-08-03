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
	params.AddIgnoredActor(Hands[0]);
	params.AddIgnoredActor(Hands[1]);
	// ���� ��� ������ �κ�
	endPos = playerComp->endPos;

	// hit�� ��ü ���� ������
	FHitResult hitInfo;
	GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, params);
	// ���̸� ���
	bool bhit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, params);

	if (hitInfo.GetActor() != nullptr && hitInfo.GetActor()->ActorHasTag("E_Bat"))
	{
		// ���� �ִ� ���ڿ� ���͸��� ������� ���� ���
		auto* bat = hitInfo.GetActor()->FindComponentByClass<UKMK_Bat>();
		FVector dir = hitInfo.GetActor()->GetTargetLocation() - playerComp->startPos;
		// �����Ÿ� ���ζ��, ���͸��� ���� �� �ִ� ���°� ��
		// GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Orange, FString::Printf(TEXT("%f, %s"), dir.Length(), *hitInfo.GetActor()->GetActorLabel()));
		if (!bat->isCome)
		{
			// �����Ÿ� ���ζ��, ���͸��� ���� �� �ִ� ���°� ��
			if (dir.Length() < PDis)
			{
				Hands[0]->isCome = true;
				Hands[1]->isCome = true;
			}
			// Ŭ���� �ǰ�
			if (isRay)
			{	
				
				// ���͸��� ��� �ִ� ���� �ľ��� ���͸��� �־���
				for (int i = 0; i < 2; i++)
				{
					if (playerComp->isDir[i] && Hands[i]->isGrab)
					{
						playerComp->Hands[i]->isBatCom = true;
						bat->isCome = true;
						Hands[i]->isGrab = false;
						Hands[i]->isCome = false;
						Bats[i]->SetVis(false);
					}
				}
			}
			
		}
		// ���͸��� ���ڿ� ����ִ� ���
		else
		{
			
			// �Ÿ��� Ȯ������, ���ͷ����� �����ϰ� ����
			if (playerComp->isDir[0])
			{
				bat->isCome = false;
				Bats[0]->SetVis(true);
				Hands[0]->isCome = false;
			}
			if(playerComp->isDir[1])
			{
				bat->isCome = false;
				Bats[1]->SetVis(true);
				Hands[1]->isCome = false;
			}
		}

	}
	// �Է°��� ������ ���
	if (isRay)
	{
		// ���̸� �׸���
		DrawDebugLine(GetWorld(), startPos, endPos, FColor::Blue, false, 1.f);
		//  ������ �̶��
		if(playerComp->isDir[0])
		{
			
			// ���̿� ��ü�� ���� ���
			if(bhit)
			{
				// ���� ��� �ִٸ�
				if (playerComp->RMeshComp->GetStaticMesh() == Hands[0]->HandMesh[1])
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
					Hands[0]->hitinfo = hitInfo.GetComponent();
					// ���� ���� �����
					// �� �ʱⰪ
					Hands[0]->handPos = 27;
					// ���� ��������
					Hands[0]->endPos = hitInfo.ImpactPoint;
					// ���� ���� ����
					if (Hands[0]->isGrab || Hands[0]->isPick)
					{
						Hands[0]->isGrab = false;
						Hands[0]->isPick = false;
					}
					else Hands[0]->isGo = true;
				}
			}
			// ��ü�� ���� ���� ���
			else
			{
				if (Hands[0]->isGrab)
				{
					Hands[0]->isGrab = false;
					return;
				}

				// 1. ���� ��� ���� �ʴٸ�
				if (playerComp->RMeshComp->GetStaticMesh() != Hands[0]->HandMesh[1])
				{
					// ���� ���´�
					Hands[0]->handPos = 27;
					Hands[0]->endPos = playerComp->endPos;
					Hands[0]->isGo = true;
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
		if (playerComp->isDir[1])
		{

			if (Hands[1]->isGrab)
			{
				Hands[1]->isGrab = false;
				return;
			}
			// ��ü�� �ִٸ�
			if (bhit)
			{
				Hands[1]->endPos = hitInfo.ImpactPoint;
				Hands[1]->hitinfo = hitInfo.GetComponent();
			}
			else Hands[1]->endPos = playerComp->endPos;
			// ���������� ����
			Hands[1]->handPos = -27;
			Hands[1]->isRay = true;
			if (Hands[1]->isGrab || Hands[1]->isPick)
			{
				Hands[1]->isGrab = false;
				Hands[1]->isPick = false;
			}
			else Hands[1]->isGo = true;
		}
	}
}

void UKMK_PlayerRay::SetRayPos(FVector start, FVector end)
{
	startPos = start;
	endPos = end;

	isRay = true;
}

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
	/*FHitResult hitInfo1;
	GetWorld()->LineTraceSingleByChannel(hitInfo1, startPos, endPos, ECC_EngineTraceChannel1, params);*/
	GetWorld()->LineTraceSingleByChannel(hitInfo, playerComp->startPos, endPos, ECC_GameTraceChannel8, params);
	// DrawDebugLine(GetWorld(), playerComp->startPos, endPos, FColor::Blue, false, 1.f);
	if(hitInfo.GetActor() != nullptr)GEngine->AddOnScreenDebugMessage(2, 1, FColor::Orange, FString::Printf(TEXT("%s"), *hitInfo.GetActor()->GetName()));
	// ���̸� ���
	bool bhit = GetWorld()->LineTraceSingleByChannel(hitInfo, playerComp->startPos, endPos, ECC_GameTraceChannel8, params);

	if (hitInfo.GetActor() != nullptr && hitInfo.GetActor()->ActorHasTag("E_Bat"))
	{
		GEngine->AddOnScreenDebugMessage(2, 1, FColor::Orange, FString::Printf(TEXT("%f"), FVector::Distance(hitInfo.GetActor()->GetActorLocation(), GetOwner()->GetTargetLocation())));
		// �����Ÿ��� ������ ���ͷ����� �����ϰ� ����
		if (FVector::Distance(hitInfo.GetActor()->GetActorLocation(), GetOwner()->GetTargetLocation()) < 400)
		{
			isCome = true;
			Hands[0]->isCome = true;
			Hands[1]->isCome = true;
		}
		// ���� �ִ� ���ڿ� ���͸��� ������� ���� ���
		auto* bat = hitInfo.GetActor()->FindComponentByClass<UKMK_Bat>();

		// �ϰ���� �� : ���͸��� �־��� ���ٰ� �ϰ� �ϰ� �ʹ�
		// �ؾ��ϴ� ��
		// 1. ���ͷ����� �����ϰ�
		if (isCome && isRay)
		{
			// 2. ���ڿ� ���͸��� ���� ���
			if(!bat->isHaveBat)
			{ 
				// 2-1. �տ� ���͸��� �ִٸ�
				for (int i = 0; i < 2; i++)
				{
					if (Hands[i]->isGrab)
					{
						// 2-2. �ְ� �ʹ�
						bat->isHaveBat = true;
						// 2-3. �տ� �ִ� ���͸��� ���� �ʹ�.
						Bats[i]->SetVis(false);
						// 2-4. ���� grab���°� �ƴϰ� �ϰ� �ʹ�
						Hands[i]->isGrab = false;
					}
				}
			}
			// 3. ���ڿ� ���͸��� �ִ� ���
			else
			{
				// 3-1. �տ� ���͸��� ������
				if (bat->isHaveBat && isRay)
				{
					for (int i = 0; i < 2; i++)
					{
						// 3-2. �տ� ���͸��� �ְ� �ʹ�.
						if (playerComp->isDir[i])
						{
							if (!Hands[i]->isGrab)
							{
								bat->isHaveBat = false;
								// 2-2. �ְ� �ʹ�
								Bats[i]->SetVis(true);
								Hands[i]->isGrab = true;
							}
							else
							{
								Bats[i]->SetVis(true);
								Hands[i]->isGrab = true;
							}
						}
					}
				}
			}
		}
	}
	else
	{
		isCome = false;
		Hands[0]->isCome = false;
		Hands[1]->isCome = false;
	}

	// �Է°��� ������ ���
	if (isRay)
	{
		// ���̸� �׸���
		if(hitInfo.GetActor() != nullptr) DrawDebugLine(GetWorld(), startPos, endPos, FColor::Blue, false, 1.f);
		if (isCome)
		{
			isCome = false;
			return;
		}
		//  ������ �̶��
		if(playerComp->isDir[0])
		{	
			if(Hands[0]->isCome) Hands[0]->isCome = false;
			
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
				
					if (hitInfo.GetActor()->ActorHasTag("Jump") && FVector::Distance(hitInfo.GetActor()->GetActorLocation(), GetOwner()->GetActorLocation()) < 300)
					{
						Hands[0]->isJump = true;
					}

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
			if (Hands[1]->isCome) Hands[1]->isCome = false;
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

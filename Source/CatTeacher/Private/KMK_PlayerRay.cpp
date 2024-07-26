// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK_PlayerRay.h"
#include "KMK_PlayerHandFSM.h"
#include "KMK_Player.h"
#include "KMK_PlayerHand.h"

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

	endPos = playerComp->endPos;
	// hit�� ��ü ���� ������
	FHitResult hitInfo;
	// Ŭ���� �ȴٸ�
	// ���̸� ���
	bool bhit = GetWorld()->LineTraceSingleByChannel(hitInfo, playerComp->startPos, endPos, ECC_Visibility, params);
	if (bhit)
	{
		// �������� �����ƴٸ�
		// �������� ������ �ִ� fsm state�� movestop���� �����Ұſ���
	}
	//DrawDebugLine(GetWorld(), playerComp->startPos, endPos, FColor::Red, false, 1.f);
	bool bhit1 = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, params);

	if (isRay)
	{
		// ��ü�� �ִٸ�
		if (bhit1)
		{
			DrawDebugLine(GetWorld(), startPos, endPos, FColor::Blue, false, 1.f);
			GEngine->AddOnScreenDebugMessage(1, 1, FColor::Blue, FString::Printf(TEXT("hi")));
			if(playerComp->isRight)
			{
				// energy�е� ���������
				if (playerComp->RMeshComp->GetStaticMesh() == playerComp->RHand->HandMesh[0])
				{
					if (hitInfo.GetActor()->GetActorLabel().Contains("ElectricalPanel"))
					{
						FSM->PState = PlayerHandFSM::Energy;
						FSM->t = 0;
						FSM->isCharge = true;
					}
					else
					{
						FVector s = playerComp->RHand->GetActorLocation();
						FVector e = hitInfo.ImpactPoint;
						FVector dir = e - s;
						playerComp->RHand->SetActorLocation(playerComp->RHand->GetActorLocation() + dir * 100 * DeltaTime );
					}
				}

				// jump�е带 ����������
				if (playerComp->RMeshComp->GetStaticMesh() == playerComp->RHand->HandMesh[1])
				{
					FSM->bulletTrans = hitInfo.ImpactPoint;
					FSM->isFire	= true;
					FSM->PState = PlayerHandFSM::GunPack;
				}
				// jump�е带 ����������
				if (playerComp->RMeshComp->GetStaticMesh() == playerComp->RHand->HandMesh[2] && hitInfo.GetActor()->GetActorLabel().Contains("jump"))
				{
					FSM->isJump = false;
					FSM->PState = PlayerHandFSM::JumpPack;
				}

			}
		}
	}
}

void UKMK_PlayerRay::SetRayPos(FVector start, FVector end)
{
	startPos = start;
	endPos = end;

	isRay = true;
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK_PlayerRay.h"
#include "KMK_PlayerHandFSM.h"
#include "KMK_Player.h"
#include "KMK_PlayerHand.h"
#include "../../../../Plugins/Runtime/CableComponent/Source/CableComponent/Classes/CableComponent.h"

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

		DrawDebugLine(GetWorld(), startPos, endPos, FColor::Blue, false, 1.f);
		GEngine->AddOnScreenDebugMessage(1, 1, FColor::Blue, FString::Printf(TEXT("hi")));
		if(playerComp->isRight)
		{
			// energy�е� ���������
			if (playerComp->RMeshComp->GetStaticMesh() == playerComp->RHand->HandMesh[0])
			{
				playerComp->RHand->handPos = FVector(30, 20, -13);
				if (bhit1)playerComp->RHand->endPos = hitInfo.ImpactPoint;
				else playerComp->RHand->endPos = playerComp->endPos;
				playerComp->RHand->isGo = true;

				if (bhit1 && hitInfo.GetActor()->GetActorLabel().Contains("ElectricalPanel"))
				{
					FSM->PState = PlayerHandFSM::Energy;
					FSM->t = 0;
					FSM->isCharge = true;
				}
			}

			// jump�е带 ����������
			if (bhit1 && playerComp->RMeshComp->GetStaticMesh() == playerComp->RHand->HandMesh[1])
			{
				FSM->bulletTrans = hitInfo.ImpactPoint;
				FSM->isFire	= true;
				FSM->PState = PlayerHandFSM::GunPack;
			}
			// jump�е带 ����������
			if (playerComp->RMeshComp->GetStaticMesh() == playerComp->RHand->HandMesh[2] && hitInfo.GetActor()->GetActorLabel().Contains("jump"))
			{
				playerComp->RHand->handPos = FVector(30, 20, -13);
				if (bhit1)playerComp->RHand->endPos = hitInfo.ImpactPoint;
				else playerComp->RHand->endPos = playerComp->endPos;
				playerComp->RHand->isGo = true;
				FSM->isJump = false;
				FSM->PState = PlayerHandFSM::JumpPack;
			}

		}
		if (playerComp->isLeft)
		{
			playerComp->LHand->handPos = FVector(30, -20, -13);
			if (bhit1)playerComp->LHand->endPos = hitInfo.ImpactPoint;
			else playerComp->LHand->endPos = playerComp->endPos;
			playerComp->LHand->isGo = true;
		}

		
	}
}

void UKMK_PlayerRay::SetRayPos(FVector start, FVector end)
{
	startPos = start;
	endPos = end;

	isRay = true;
}



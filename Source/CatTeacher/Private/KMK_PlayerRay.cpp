// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK_PlayerRay.h"
#include "KMK_PlayerHandFSM.h"
#include "KMK_Player.h"

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
	
	endPos = playerComp->endPos;
	// hit�� ��ü ���� ������
	FHitResult hitInfo;
	// Ŭ���� �ȴٸ�
	// ���̸� ���
	bool bhit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, params);

	if (isRay)
	{
		// ��ü�� �ִٸ�
		if (bhit)
		{
			DrawDebugLine(GetWorld(), startPos, endPos, FColor::Blue, false, 5.f);
			GEngine->AddOnScreenDebugMessage(1, 1, FColor::Blue, FString::Printf(TEXT("hi")));
			if(playerComp->isRight)
			{
				// energy�е� ���������
				if (playerComp->Rhand->GetStaticMesh() == playerComp->RHandMeshes[0] && hitInfo.GetActor()->GetActorLabel().Contains("ElectricalPanel"))
				{
					FSM->PState = PlayerHandFSM::Energy;
					FSM->isCharge = true;
				}
				// jump�е带 ����������
				if (playerComp->Rhand->GetStaticMesh() == playerComp->RHandMeshes[2] && hitInfo.GetActor()->GetActorLabel().Contains("jump"))
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


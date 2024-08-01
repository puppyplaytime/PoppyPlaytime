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
	// 레이 셋팅
	FCollisionQueryParams params;
	params.AddIgnoredActor(GetOwner());
	params.AddIgnoredActor(playerComp->RHand);
	params.AddIgnoredActor(playerComp->LHand);
	// 레이 쏘는 마지막 부분
	endPos = playerComp->endPos;

	// hit된 물체 정보 들고오기
	FHitResult hitInfo;

	// 레이를 쏘고
	bool bhit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, params);
	//DrawDebugLine(GetWorld(), playerComp->startPos, endPos, FColor::Red, false, 1.f);
	// 클릭정보가 들어온다면,

	if (isRay)
	{
		// 레이를 그리고
		DrawDebugLine(GetWorld(), startPos, endPos, FColor::Blue, false, 1.f);
		//  오른손 이라면
		if(playerComp->isRight)
		{
			// 레이에 물체가 맞은 경우
			if(bhit)
			{
				// 총을 들고 잇따면
				if (playerComp->RMeshComp->GetStaticMesh() == playerComp->RHand->HandMesh[1])
				{
					// 총을 쏜다
					FSM->isFire	= true;
					// FSM을 gunpack으로 변경하고
					FSM->PState = PlayerHandFSM::GunPack;
				}
				// 총을 들고있지 않다면
				else
				{
					// 오른손에 물체의 component를 넘겨주고
					playerComp->RHand->hitinfo = hitInfo.GetComponent();
					// 손을 뻗게 만든다
					// 손 초기값
					playerComp->RHand->handPos = 27;
					// 손의 도착지점
					playerComp->RHand->endPos = hitInfo.ImpactPoint;
					// 손을 뻗게 만듦
					if (playerComp->RHand->isGrab || playerComp->RHand->isPick)
					{
						playerComp->RHand->isGrab = false;
						playerComp->RHand->isPick = false;
					}
					else playerComp->RHand->isGo = true;
				}
			}
			// 물체가 맞지 않은 경우
			else
			{
				if (playerComp->RHand->isGrab)
				{
					playerComp->RHand->isGrab = false;
					return;
				}
				// 1. 총을 들고 있지 않다면
				if (playerComp->RMeshComp->GetStaticMesh() != playerComp->RHand->HandMesh[1])
				{
					// 손을 뻗는다
					playerComp->RHand->handPos = 27;
					playerComp->RHand->endPos = playerComp->endPos;
					playerComp->RHand->isGo = true;
				}
				// 아니라면
				else
				{
					// 총쏘기
					FSM->isFire = true;
				}
			}
		}
		// 왼손이라면
		if (playerComp->isLeft)
		{
			if (playerComp->LHand->isGrab)
			{
				playerComp->LHand->isGrab = false;
				return;
			}
			// 물체가 있다면
			if (bhit)
			{
				playerComp->LHand->endPos = hitInfo.ImpactPoint;
				playerComp->LHand->hitinfo = hitInfo.GetComponent();
			}
			else playerComp->LHand->endPos = playerComp->endPos;
			// 공통적으로 수행
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


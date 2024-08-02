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
	// 레이 셋팅
	FCollisionQueryParams params;
	params.AddIgnoredActor(GetOwner());
	params.AddIgnoredActor(playerComp->Hands[0]);
	params.AddIgnoredActor(playerComp->Hands[1]);
	// 레이 쏘는 마지막 부분
	endPos = playerComp->endPos;

	// hit된 물체 정보 들고오기
	FHitResult hitInfo;

	// 레이를 쏘고
	bool bhit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, params);
	//DrawDebugLine(GetWorld(), playerComp->startPos, endPos, FColor::Red, false, 1.f);
	// 클릭정보가 들어온다면,

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
		// 레이를 그리고
		DrawDebugLine(GetWorld(), startPos, endPos, FColor::Blue, false, 1.f);
		cnt++;
		//  오른손 이라면
		if(playerComp->isRight)
		{
			// 레이에 물체가 맞은 경우
			if(bhit)
			{
				// 총을 들고 잇따면
				if (playerComp->RMeshComp->GetStaticMesh() == playerComp->Hands[0]->HandMesh[1])
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
					playerComp->Hands[0]->hitinfo = hitInfo.GetComponent();
					// 손을 뻗게 만든다
					// 손 초기값
					playerComp->Hands[0]->handPos = 27;
					// 손의 도착지점
					playerComp->Hands[0]->endPos = hitInfo.ImpactPoint;
					// 손을 뻗게 만듦
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
			// 물체가 맞지 않은 경우
			else
			{
				if (playerComp->Hands[0]->isGrab)
				{
					playerComp->Hands[0]->isGrab = false;
					return;
				}
				// 1. 총을 들고 있지 않다면
				if (playerComp->RMeshComp->GetStaticMesh() != playerComp->Hands[0]->HandMesh[1])
				{
					// 손을 뻗는다
					playerComp->Hands[0]->handPos = 27;
					playerComp->Hands[0]->endPos = playerComp->endPos;
					playerComp->Hands[0]->isGo = true;
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
			if (playerComp->Hands[1]->isGrab)
			{
				playerComp->Hands[1]->isGrab = false;
				return;
			}
			// 물체가 있다면
			if (bhit)
			{
				playerComp->Hands[1]->endPos = hitInfo.ImpactPoint;
				playerComp->Hands[1]->hitinfo = hitInfo.GetComponent();
			}
			else playerComp->Hands[1]->endPos = playerComp->endPos;
			// 공통적으로 수행
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
